#include "precompiled.hpp"
#include "game/physics/oimo/oimo.hpp"

#include <rapidjson/writer.h>

physics_oimo_t::physics_oimo_t(v8::Isolate* v8_isolate, float time_step) :
    m_v8_isolate(v8_isolate)
{
    init_context();
    init_engine(time_step);
}

physics_oimo_t::~physics_oimo_t(){
    m_v8_context.Reset();
    m_v8_isolate->Dispose();
}

void physics_oimo_t::init_context() {
    v8_platform_t::v8_lock_t lock(m_v8_isolate);
    m_v8_context.Reset(m_v8_isolate, v8::Context::New(m_v8_isolate));
}

void physics_oimo_t::init_engine(float time_step) {
    std::string oimo_src;
    std::ifstream input("./web/oimo.min.js");
    input.seekg(0, std::ios::end);
    auto size = input.tellg();
    input.seekg(0, std::ios::beg);
    oimo_src.resize(size);
    input.read(&oimo_src[0], size);
    exec_js(oimo_src);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.StartObject();

    writer.String("gravity");
    writer.StartArray();
    writer.Double(0.0);
    writer.Double(-10.0);
    writer.Double(0.0);
    writer.EndArray();

    writer.String("info");
    writer.Bool(false);

    writer.String("random");
    writer.Bool(false);

    writer.String("worldscale");
    writer.Double(1.0);

    writer.String("timeStep");
    writer.Double(time_step);

    writer.EndObject();
    exec_js("let bodies = {}; let world = new OIMO.World(" + std::string(buffer.GetString()) + ");");
}

void physics_oimo_t::exec_js(const std::string& script, const std::function<void(const v8::Local<v8::Value>&, const v8::Local<v8::Context>&)> callback) {
    v8_platform_t::v8_lock_t lock(m_v8_isolate);

    v8::Local<v8::Context> context = v8::Local<v8::Context>::New(m_v8_isolate, m_v8_context);
    v8::Context::Scope context_scope(context);

    auto v8_str = v8::String::NewFromUtf8(m_v8_isolate, script.c_str(), v8::NewStringType::kNormal, script.size()).ToLocalChecked();
    auto v8_script = v8::Script::Compile(context, v8_str).ToLocalChecked();
    v8::MaybeLocal<v8::Value> script_res = v8_script->Run(context);

    if (callback) {
        v8::Local<v8::Value> res;
        if (script_res.ToLocal(&res)) {
            callback(script_res.ToLocalChecked(), context);
        } else {
            callback(res, context);
        }
    }
}

void physics_oimo_t::add_body(const engine::physics::object_t::body_t& body) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.StartObject();

    writer.String("type");
    writer.StartArray();
    for (const auto& shape : body.get_shapes()) {
        switch (shape->type) {
            case engine::physics::object_t::shape_type::sphere: {
                writer.String("sphere");
                break;
            }
            case engine::physics::object_t::shape_type::box: {
                writer.String("box");
                break;
            }
            case engine::physics::object_t::shape_type::cylinder: {
                writer.String("cylinder");
                break;
            }
        }
    }
    writer.EndArray();

    writer.String("size");
    writer.StartArray();
    for (const auto& shape : body.get_shapes()) {
        writer.Double(shape->size.x);
        writer.Double(shape->size.y);
        writer.Double(shape->size.z);
    }
    writer.EndArray();

    writer.String("posShape");
    writer.StartArray();
    for (const auto& shape : body.get_shapes()) {
        writer.Double(shape->position.x);
        writer.Double(shape->position.y);
        writer.Double(shape->position.z);
    }
    writer.EndArray();

    writer.String("pos");
    writer.StartArray();
    const auto& position = body.get_state().position;
    writer.Double(position.x);
    writer.Double(position.y);
    writer.Double(position.z);
    writer.EndArray();

    writer.String("move");
    writer.Bool(body.is_moving());

    writer.EndObject();

    exec_js("bodies[" + std::to_string(body.get_id()) + "] = world.add(" + buffer.GetString() + ")");
}

void physics_oimo_t::destroy_body(int body_id) {
    exec_js("delete bodies[" + std::to_string(body_id) + "]");
}

void physics_oimo_t::update_body_state(engine::physics::object_t::body_t& out_body) {
    exec_js("bodies[" + std::to_string(out_body.get_id()) + "]", [&](const v8::Local<v8::Value>& value, const v8::Local<v8::Context>& context) {
        auto body = value->ToObject(context).ToLocalChecked();
        auto field_x = v8::String::NewFromUtf8(m_v8_isolate, "x").ToLocalChecked();
        auto field_y = v8::String::NewFromUtf8(m_v8_isolate, "y").ToLocalChecked();
        auto field_z = v8::String::NewFromUtf8(m_v8_isolate, "z").ToLocalChecked();
        auto field_w = v8::String::NewFromUtf8(m_v8_isolate, "w").ToLocalChecked();
        glm::vec3 position;
        glm::vec4 rotation;
        glm::vec3 linear_speed;
        glm::vec3 angular_speed;
        auto field = body->Get(context, v8::String::NewFromUtf8(m_v8_isolate, "position").ToLocalChecked()).ToLocalChecked()->ToObject(context).ToLocalChecked();
        position.x = field->Get(context, field_x).ToLocalChecked()->NumberValue(context).ToChecked();
        position.y = field->Get(context, field_y).ToLocalChecked()->NumberValue(context).ToChecked();
        position.z = field->Get(context, field_z).ToLocalChecked()->NumberValue(context).ToChecked();
        field = body->Get(context, v8::String::NewFromUtf8(m_v8_isolate, "quaternion").ToLocalChecked()).ToLocalChecked()->ToObject(context).ToLocalChecked();
        rotation.x = field->Get(context, field_x).ToLocalChecked()->NumberValue(context).ToChecked();
        rotation.y = field->Get(context, field_y).ToLocalChecked()->NumberValue(context).ToChecked();
        rotation.z = field->Get(context, field_z).ToLocalChecked()->NumberValue(context).ToChecked();
        rotation.w = field->Get(context, field_w).ToLocalChecked()->NumberValue(context).ToChecked();
        field = body->Get(context, v8::String::NewFromUtf8(m_v8_isolate, "linearVelocity").ToLocalChecked()).ToLocalChecked()->ToObject(context).ToLocalChecked();
        linear_speed.x = field->Get(context, field_x).ToLocalChecked()->NumberValue(context).ToChecked();
        linear_speed.y = field->Get(context, field_y).ToLocalChecked()->NumberValue(context).ToChecked();
        linear_speed.z = field->Get(context, field_z).ToLocalChecked()->NumberValue(context).ToChecked();
        field = body->Get(context, v8::String::NewFromUtf8(m_v8_isolate, "angularVelocity").ToLocalChecked()).ToLocalChecked()->ToObject(context).ToLocalChecked();
        angular_speed.x = field->Get(context, field_x).ToLocalChecked()->NumberValue(context).ToChecked();
        angular_speed.y = field->Get(context, field_y).ToLocalChecked()->NumberValue(context).ToChecked();
        angular_speed.z = field->Get(context, field_z).ToLocalChecked()->NumberValue(context).ToChecked();

        auto& state = out_body.get_state();
        state.position = position;
        state.rotation = rotation;
        state.linear_speed = linear_speed;
        state.angular_speed = angular_speed;
    });
}

void physics_oimo_t::update() {
    exec_js("world.step()");
}
