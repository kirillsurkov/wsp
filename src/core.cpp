#include "precompiled.hpp"
#include "session.hpp"
#include "player.hpp"
#include "core.hpp"
#include "player_delta.hpp"
#include "messages_out.hpp"

core_t::core_t(physics_t& physics) :
	m_physics(physics),
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
	std::vector<physics_t::object_t> objects;

	std::shared_ptr<physics_t::shape_t> ground_shape = std::make_shared<physics_t::shape_t>();
	ground_shape->type = physics_t::body_type::box;
	ground_shape->position = glm::vec3(0.0, 0.0, 0.0);
	ground_shape->rotation = glm::vec4(0.0, 0.0, 0.0, 1.0);
	ground_shape->size = glm::vec3(200.0, 1.0, 200.0);

	physics_t::object_t ground;
	ground.id = m_physics_id_manager.next_id();
	ground.moving = false;
	ground.position = glm::vec3(0, 0, -40.0);
	ground.rotation = glm::vec4(0, 0, 0.0, 1.0);
	ground.rotation_factor = glm::vec3(1.0, 1.0, 1.0);
	ground.parts.push_back(ground_shape);

	m_physics.add_body(ground);
	objects.push_back(ground);

	for (int i = 0; i < 10; i++) {
	    for (int j = 0; j < 10; j++) {
	    	std::shared_ptr<physics_t::shape_t> box_shape = std::make_shared<physics_t::shape_t>();
	    	box_shape->type = physics_t::body_type::box;
	    	box_shape->position = glm::vec3(0.0, 0.0, 0.0);
			box_shape->rotation = glm::vec4(0.0, 0.0, 0.0, 1.0);
			box_shape->size = glm::vec3(2.0, 2.0, 2.0);

			physics_t::object_t box;
			box.id = m_physics_id_manager.next_id();
			box.moving = true;
			box.position = glm::vec3(-15 + i * 3 + j * 0.5, 1.0 + j * 3, -40.0 - i);
			box.rotation = glm::vec4(0.0, 0.0, 0.0, 1.0);
			box.rotation_factor = glm::vec3(1.0, 1.0, 1.0);
			box.parts.push_back(box_shape);

            m_physics.add_body(box);
            objects.push_back(box);
        }
	}

	for (const physics_t::object_t& object : objects) {
		auto message = std::make_shared<message::out::new_physics_object_t>(m_physics.get_current_frame(), object);
		for (auto& pair : m_sessions) {
			pair.second->send_message(message);
		}
	}
}

void core_t::main_loop() {
	float physics_timer = 0;
	float player_update_timer = 0;
	auto last_time = std::chrono::system_clock::now();
    while (true) {
    	auto now = std::chrono::system_clock::now();
    	float delta = std::chrono::duration_cast<std::chrono::microseconds>(now - last_time).count() / 1000000.0f;
        last_time = now;

        physics_timer += delta;
        player_update_timer += delta;

        float physics_time_step = m_physics.get_time_step();
    	while (physics_timer >= physics_time_step) {
    		physics_timer -= physics_time_step;
    		m_physics.update();
    	}

    	float player_update_time_step = 0.2f;
		if (player_update_timer >= player_update_time_step) {
			player_update_timer -= player_update_time_step;
			auto message = std::make_shared<message::out::physics_state_t>(m_physics.get_current_frame(), m_physics.get_world_state());
			for (auto& pair : m_sessions) {
				pair.second->send_message(message);
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
    int body_id = player->get_body_id();

    m_objects_id_manager.free_id(object_id);
    m_physics_id_manager.free_id(body_id);
    m_players_id_manager.free_id(player_id);

	m_objects.erase(object_id);
	m_player_to_object.erase(player_id);
	m_sessions.erase(player_id);
	m_physics.destroy_body(body_id);
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

    m_objects[object_id] = std::make_shared<player_t>(object_id, body_id, player_id, message.get_name());
    m_player_to_object[player_id] = object_id;
	m_sessions[player_id] = session;
    m_physics.add_player(body_id);
    session->set_player(player_id);

    LOG << "New player " << message.get_name();

    session->send_message(std::make_shared<message::out::login_t>(m_physics.get_current_frame()));

    if (message.get_name() == "glowstick") {
    	init_physics();
    }
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::delta_state_t& message) {
	std::cout << "Delta state" << std::endl;
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::chat_local_t& message) {
	std::cout << "Chat local" << std::endl;
	auto out_message = std::make_shared<message::out::chat_local_t>(session->get_player(), message.get_text());
	std::lock_guard<std::mutex> lock(m_objects_lock);
	for (auto& pair : m_sessions) {
		pair.second->send_message(out_message);
	}
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::chat_global_t& message) {
	std::cout << "Chat global" << std::endl;
}
