#pragma once

#include <functional>
#include <unordered_map>

#include "engine/physics/physics.hpp"
#include "game/v8/platform.hpp"

class physics_oimo_t : public engine::physics_t {
private:
    void init_context();
    void init_engine(float time_step);
    void exec_js(const std::string& script, const std::function<void(const v8::Local<v8::Value>&, const v8::Local<v8::Context>&)> callback = nullptr);

    v8::Isolate* m_v8_isolate;
    v8::Global<v8::Context> m_v8_context;

public:
    physics_oimo_t(v8::Isolate* v8_isolate, float time_step);
    virtual ~physics_oimo_t() override;

    virtual void add_body(const engine::physics::object_t::body_t& body) override;
    virtual void destroy_body(int body_id) override;

    virtual void update_body_state(engine::physics::object_t::body_t& body) override;

    virtual void update() override;
};
