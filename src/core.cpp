#include "precompiled.hpp"
#include "session.hpp"
#include "object.hpp"
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

	physics_t::object_t object = {};
	object.id = m_id_manager.next_id();
	object.type = physics_t::body_type::box;
	object.size = glm::vec3(100.0, 1.0, 100.0);
	object.position = glm::vec3(0, 0, -40.0);
	object.moving = false;
	m_physics.add_body(object);
	objects.push_back(object);
	object.size = glm::vec3(1.0);
	object.moving = true;
	for (int i = 0; i < 10; i++) {
	    for (int j = 0; j < 10; j++) {
            object.id = m_id_manager.next_id();
            object.position = glm::vec3(-15 + i * 3 + j * 0.5, 1.0 + j*3, -40.0 - i);
            m_physics.add_body(object);
            objects.push_back(object);
        }
	}

	for (const physics_t::object_t& object : objects) {
		auto message = std::make_shared<message::out::new_physics_object_t>(m_physics.get_current_frame(), object);
		for (int id : m_players) {
			auto player = std::static_pointer_cast<player_t>(m_objects[id]);
			player->get_session()->send_message(message);
		}
	}
}

void core_t::main_loop() {
	float timer = 0;
	auto last_time = std::chrono::system_clock::now();
    while (true) {
    	auto now = std::chrono::system_clock::now();
    	float delta = std::chrono::duration_cast<std::chrono::microseconds>(now - last_time).count() / 1000000.0f;
        last_time = now;
    	timer += delta;
    	std::unordered_map<int, std::unordered_map<int, double>> distances;
    	{
			std::lock_guard<std::mutex> lock(m_players_lock);
			for (int id1 : m_players) {
				auto player1 = std::static_pointer_cast<player_t>(m_objects[id1]);
				for (int id2 : m_players) {
					auto player2 = std::static_pointer_cast<player_t>(m_objects[id2]);
					distances[id1][id2] = 0;
				}
			}

			m_physics.update(delta);

			if (timer >= 0.2f) {
				timer -= 0.2f;
				auto message = std::make_shared<message::out::physics_state_t>(m_physics.get_current_frame(), m_physics.get_timer(), m_physics.get_world_state());
				for (int player_id : m_players) {
					auto player = std::static_pointer_cast<player_t>(m_objects[player_id]);
					player->get_session()->send_message(message);
				}
			}
    	}
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void core_t::on_disconnect(std::shared_ptr<session_t> session) {
    int id = session->get_id();
    if (id && m_players.count(id)) {
        std::lock_guard<std::mutex> lock(m_players_lock);
        std::cout << "Player " << std::static_pointer_cast<player_t>(m_objects[id])->get_name() << " disconnected" << std::endl;
        m_objects.erase(id);
        m_players.erase(id);
        m_id_manager.free_id(id);
    }
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::login_t& message) {
    std::lock_guard<std::mutex> lock(m_players_lock);
    int id = m_id_manager.next_id();
    session->set_id(id);
    m_objects[id] = std::make_shared<player_t>(session, message.get_name());
    m_players.insert(id);

    std::cout << "New player " << message.get_name() << std::endl;

    session->send_message(std::make_shared<message::out::login_t>(m_physics.get_current_frame() - 5));

    if (message.get_name() == "glowstick") {
    	init_physics();
    }
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::delta_state_t& message) {
	std::cout << "Delta state" << std::endl;
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::chat_local_t& message) {
	std::cout << "Chat local" << std::endl;
	int sender = session->get_id();
	const std::string& text = message.get_text();
	std::lock_guard<std::mutex> lock(m_players_lock);
	for (int id : m_players) {
		auto player = std::static_pointer_cast<player_t>(m_objects[id]);
		player->get_session()->send_message(std::make_shared<message::out::chat_local_t>(sender, text));
	}
}

void core_t::on_message(std::shared_ptr<session_t> session, const message::in::chat_global_t& message) {
	std::cout << "Chat global" << std::endl;
}
