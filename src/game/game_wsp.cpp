#include "precompiled.hpp"
#include "game/game_wsp.hpp"

#include <functional>

game_wsp_t::game_wsp_t() {
}

game_wsp_t::~game_wsp_t() {
}

void game_wsp_t::create() {
    set_messages_receiver(shared_from_this());
}

void game_wsp_t::on_disconnect(std::shared_ptr<engine::network::session_t>& session) {
    int player_id = session->get_player();
    if (!player_id) {
        return;
    }

    /*std::lock_guard<std::mutex> lock(m_objects_lock);

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

    std::cout << "Player " << player->get_name() << " disconnected" << std::endl;*/
}

void game_wsp_t::on_message(std::shared_ptr<engine::network::session_t>& session, const message::in::login_t& message) {
    std::cout << "message::in::login_t: " << message.get_name() << std::endl;

    /*if (session->get_player()) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_objects_lock);

    int object_id = m_objects_id_manager.next_id();
    int body_id = m_physics_id_manager.next_id();
    int player_id = m_players_id_manager.next_id();

    auto player = std::make_shared<player_t>(object_id, body_id, player_id, message->get_name());
    m_objects[object_id] = player;
    m_player_to_object[player_id] = object_id;
    m_sessions[player_id] = session;
    m_physics->add_body(player->get_body());
    session->set_player(player_id);

    LOG << "New player " << player->get_name();

    session->send_message(std::make_shared<message::out::login_t>(m_current_frame));

    if (player->get_name() == "glowstick") {
        init_physics();
    }*/
}

void game_wsp_t::on_message(std::shared_ptr<engine::network::session_t>& session, const message::in::delta_state_t& message) {
    std::cout << "Delta state" << std::endl;
}

void game_wsp_t::on_message(std::shared_ptr<engine::network::session_t>& session, const message::in::chat_local_t& message) {
    std::cout << "Chat local" << std::endl;
    /*auto out_message = std::make_shared<message::out::chat_local_t>(m_current_frame, session->get_player(), message->get_text());
    std::lock_guard<std::mutex> lock(m_objects_lock);
    for (auto& pair : m_sessions) {
        pair.second->send_message(out_message);
    }*/
}

void game_wsp_t::on_message(std::shared_ptr<engine::network::session_t>& session, const message::in::chat_global_t& message) {
    std::cout << "Chat global" << std::endl;
}
