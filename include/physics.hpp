#pragma once

#include <v8.h>
#include <libplatform/libplatform.h>

#include <glm/glm.hpp>

class physics_t {
public:
    enum class body_type {
        sphere,
        box,
        cylinder
    };

    struct shape_t {
        body_type type;
        glm::vec3 position;
        glm::vec4 rotation;
        glm::vec3 size;
    };

    struct object_t {
        int id;
        bool moving;
        glm::vec3 position;
        glm::vec4 rotation;
        glm::vec3 rotation_factor;
        std::vector<std::shared_ptr<shape_t>> parts;
    };

    struct object_state_t {
        glm::vec3 position;
        glm::vec4 rotation;
        glm::vec3 linear_speed;
        glm::vec3 angular_speed;
    };

private:
    class v8_lock_t {
    private:
        v8::Locker m_lock;
        v8::Isolate::Scope m_isolate_scope;
        v8::HandleScope m_handle_scope;

    public:
        v8_lock_t(v8::Isolate* isolate) :
            m_lock(isolate),
            m_isolate_scope(isolate),
            m_handle_scope(isolate)
        {
        }
    };

    v8::Isolate* m_v8_isolate;
    v8::Global<v8::Context> m_v8_context;
    float m_time_step;
    int m_counter;

    void init_context();
    void init_engine();
    void exec_js(const std::string& script, const std::function<void(const v8::Local<v8::Value>&, const v8::Local<v8::Context>&)> callback = nullptr);

public:
    physics_t(v8::Isolate* v8_isolate);
    ~physics_t();

    void add_body(const object_t& object);
    void add_player(int body_id);

    void destroy_body(int body_id);

    void update();

    std::unordered_map<int, physics_t::object_state_t> get_world_state();

    float get_time_step() const;
    int get_current_frame() const;
};
