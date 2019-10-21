#include "precompiled.hpp"
#include "session.hpp"
#include "player.hpp"
#include "core.hpp"
#include "player_delta.hpp"
#include "messages_out.hpp"
#include "game_objects/box.hpp"

core_t::core_t(std::shared_ptr<physics_t> physics, float time_step) :
    m_physics(physics),
    m_time_step(time_step),
    m_current_frame(0),
    m_main_loop([&]() {
        main_loop();
    })
{
    LOG << "Core system started";
}

core_t::~core_t() {
    if (m_main_loop.joinable()) {
        m_main_loop.join();
    }
    LOG << "Core system stopped";
}

void core_t::init_physics() {
    auto ground = std::make_shared<box_t>(m_objects_id_manager.next_id(), m_physics_id_manager.next_id(), glm::vec3(200.0f, 1.0f, 200.0f));
    ground->set_position(glm::vec3(0.0f, 0.0f, -40.0f));
    m_physics->add_body(ground->get_body());
    m_objects[ground->get_id()] = ground;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            auto box = std::make_shared<box_t>(m_objects_id_manager.next_id(), m_physics_id_manager.next_id(), glm::vec3(1.0f, 1.0f, 1.0f));
            box->set_position(glm::vec3(-15.0f + i * 3.0f + j * 0.5f, 1.0f + j * 3.0f, -40.0f - i));
            m_physics->add_body(box->get_body());
            m_objects[box->get_id()] = box;
        }
    }
}

void core_t::main_loop() {
    float fps_timer = 0;
    float player_update_timer = 0;
    auto last_time = std::chrono::system_clock::now();
    while (true) {
        auto now = std::chrono::system_clock::now();
        float delta = std::chrono::duration_cast<std::chrono::microseconds>(now - last_time).count() / 1000000.0f;
        last_time = now;

        fps_timer += delta;
        player_update_timer += delta;

        while (fps_timer >= m_time_step) {
            fps_timer -= m_time_step;
            m_physics->update();
            for (auto& object : m_objects) {
                auto physics_object = std::dynamic_pointer_cast<physics_object_t>(object.second);
            }
            m_current_frame++;
        }

        float player_update_time_step = 0.2f;
        if (player_update_timer >= player_update_time_step) {
            player_update_timer -= player_update_time_step;
            auto message = std::make_shared<message::out::physics_state_t>(m_current_frame, m_physics->get_world_state());
            for (const auto& pair_sessions : m_sessions) {
                const auto& player = std::dynamic_pointer_cast<player_t>(m_objects[m_player_to_object[pair_sessions.first]]);
                for (const auto& pair_objects : m_objects) {
                }
                pair_sessions.second->send_message(message);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void core_t::on_disconnect(std::shared_ptr<session_t> session) {
    int player_id = session->get_player();
    if (!player_id) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_objects_lock);

    int object_id = m_player_to_object[player_id];
    auto player = std::dynamic_pointer_cast<player_t>(m_objects[object_id]);
    int body_id = player->get_body().get_id();

    m_objects_id_manager.free_id(object_id);
    m_physics_id_manager.free_id(body_id);
    m_players_id_manager.free_id(player_id);

    m_objects.erase(object_id);
    m_player_to_object.erase(player_id);
    m_sessions.erase(player_id);
    m_physics->destroy_body(body_id);
    session->set_player(0);

    std::cout << "Player " << player->get_name() << " disconnected" << std::endl;
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::login_t& message) {
    if (session->get_player()) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_objects_lock);

    int object_id = m_objects_id_manager.next_id();
    int body_id = m_physics_id_manager.next_id();
    int player_id = m_players_id_manager.next_id();

    auto player = std::make_shared<player_t>(object_id, body_id, player_id, message.get_name());
    m_objects[object_id] = player;
    m_player_to_object[player_id] = object_id;
    m_sessions[player_id] = session;
    m_physics->add_body(player->get_body());
    session->set_player(player_id);

    LOG << "New player " << message.get_name();

    session->send_message(std::make_shared<message::out::login_t>(m_current_frame));

    if (message.get_name() == "glowstick") {
        init_physics();
    }
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::delta_state_t& message) {
    std::cout << "Delta state" << std::endl;
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::chat_local_t& message) {
    std::cout << "Chat local" << std::endl;
    auto out_message = std::make_shared<message::out::chat_local_t>(m_current_frame, session->get_player(), message.get_text());
    std::lock_guard<std::mutex> lock(m_objects_lock);
    for (auto& pair : m_sessions) {
        pair.second->send_message(out_message);
    }
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::chat_global_t& message) {
    std::cout << "Chat global" << std::endl;
}
