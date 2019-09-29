#include "precompiled.hpp"
#include "physics.hpp"

physics_t::physics_t(v8::Isolate* v8_isolate) :
	m_v8_isolate(v8_isolate),
	m_time_step(1.0f / 60.0f),
	m_timer(0),
	m_counter(0)
{
	init_context();
	init_engine();

	LOG << "Physics engine started";
}

physics_t::~physics_t() {
	m_v8_context.Reset();
	m_v8_isolate->Dispose();

	LOG << "Physics engine stopped";
}

void physics_t::init_context() {
	v8_lock_t lock(m_v8_isolate);
	m_v8_context.Reset(m_v8_isolate, v8::Context::New(m_v8_isolate));
}

void physics_t::init_engine() {
	std::string oimo_src;
	std::ifstream input("./web/oimo.min.js");
	input.seekg(0, std::ios::end);
	size_t size = input.tellg();
	input.seekg(0, std::ios::beg);
	oimo_src.resize(size);
	input.read(&oimo_src[0], size);

	exec_js(oimo_src);
	exec_js("let bodies = {}; let world = new OIMO.World({gravity: [0, -10, 0], info: false, random: false, worldscale: 1, timeStep: " + std::to_string(m_time_step) + "});");
}

void physics_t::exec_js(const std::string& script, const std::function<void(const v8::Local<v8::Value>&, const v8::Local<v8::Context>&)> callback) {
	v8_lock_t lock(m_v8_isolate);

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

void physics_t::add_body(const object_t& object) {
	std::string object_shape;
	switch (object.type) {
		case body_type::box: {
			object_shape = "box";
			break;
		}
		case body_type::cylinder: {
			object_shape = "cylinder";
			break;
		}
		case body_type::sphere: {
			object_shape = "sphere";
			break;
		}
	}
	std::string object_size = "[" + std::to_string(object.size.x * 2) + ", " + std::to_string(object.size.y * 2) + ", " + std::to_string(object.size.z * 2) + "]";
	std::string object_pos = "[" + std::to_string(object.position.x) + ", " + std::to_string(object.position.y) + ", " + std::to_string(object.position.z) + "]";
	exec_js("bodies[" + std::to_string(object.id) + "] = world.add({type: '" + object_shape + "', move: " + (object.moving ? "true" : "false") + ", size: " + object_size + ", pos: " + object_pos + "})");
}

void physics_t::update(float delta) {
	m_timer += delta;
	while (m_timer >= m_time_step) {
		m_timer -= m_time_step;
		m_counter++;
		exec_js("world.step()");
	}
}

std::unordered_map<int, physics_t::object_state_t> physics_t::get_world_state() {
	std::unordered_map<int, object_state_t> res;
	exec_js("bodies", [this, &res](const v8::Local<v8::Value>& value, const v8::Local<v8::Context>& context) {
		auto bodies = value->ToObject(context).ToLocalChecked();
		auto keys = bodies->GetOwnPropertyNames(context).ToLocalChecked();
		auto field_x = v8::String::NewFromUtf8(m_v8_isolate, "x").ToLocalChecked();
		auto field_y = v8::String::NewFromUtf8(m_v8_isolate, "y").ToLocalChecked();
		auto field_z = v8::String::NewFromUtf8(m_v8_isolate, "z").ToLocalChecked();
		auto field_w = v8::String::NewFromUtf8(m_v8_isolate, "w").ToLocalChecked();
		for (int i = 0, cnt = keys->Length(); i < cnt; i++) {
			glm::vec3 position;
			glm::vec4 rotation;
			glm::vec3 linear_speed;
			glm::vec3 angular_speed;
			auto id = keys->Get(context, i).ToLocalChecked();
			auto body = bodies->Get(context, id).ToLocalChecked()->ToObject(context).ToLocalChecked();
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
			res[static_cast<int>(id->NumberValue(context).ToChecked())] = object_state_t{position, rotation, linear_speed, angular_speed};
		}
	});
	return res;
}

float physics_t::get_timer() const {
    return m_timer;
}

int physics_t::get_current_frame() const {
	return m_counter;
}
