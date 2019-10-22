#pragma once

#include <thread>
#include <mutex>
#include <unordered_map>

#include "id_manager.hpp"
#include "physics.hpp"
#include "messages_receiver.hpp"

class id_manager_t;
class game_object_t;
class core_t : public messages_receiver_t {
private:
    std::shared_ptr<physics_t> m_physics;

    float m_time_step;
    int m_current_frame;

    id_manager_t m_objects_id_manager;
    id_manager_t m_physics_id_manager;
    id_manager_t m_players_id_manager;

    std::thread m_main_loop;

    std::mutex m_objects_lock;
    std::unordered_map<int, std::shared_ptr<game_object_t>> m_objects;
    std::unordered_map<int, std::shared_ptr<session_t>> m_sessions;
    std::unordered_map<int, int> m_player_to_object;

    void main_loop();

    void init_physics();

public:
    core_t(std::shared_ptr<physics_t> physics, float time_step);
    virtual ~core_t() override;

    virtual void on_disconnect(std::shared_ptr<session_t> session) override;
    virtual void on_message(std::shared_ptr<session_t> session, const std::shared_ptr<message::in::login_t>& message) override;
    virtual void on_message(std::shared_ptr<session_t> session, const std::shared_ptr<message::in::delta_state_t>& message) override;
    virtual void on_message(std::shared_ptr<session_t> session, const std::shared_ptr<message::in::chat_local_t>& message) override;
    virtual void on_message(std::shared_ptr<session_t> session, const std::shared_ptr<message::in::chat_global_t>& message) override;
};
