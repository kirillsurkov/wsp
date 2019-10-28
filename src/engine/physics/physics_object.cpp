#include "precompiled.hpp"
#include "engine/physics/physics_object.hpp"

engine::physics::object_t::body_t::body_t(int id) :
    m_id(id),
    m_moving(false),
    m_rotation_factor(glm::vec3(1.0f, 1.0f, 1.0f)),
    m_state({false,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)})
{
}

engine::physics::object_t::body_t::~body_t() {
}

void engine::physics::object_t::body_t::set_moving(bool moving) {
    m_moving = moving;
}

void engine::physics::object_t::body_t::set_rotation_factor(const glm::vec3& rotation_factor) {
    m_rotation_factor = rotation_factor;
}

void engine::physics::object_t::body_t::add_shape(shape_type type, const glm::vec3& position, const glm::vec4& rotation, const glm::vec3& size) {
    auto shape = std::make_shared<engine::physics::object_t::shape_t>();
    shape->type = type;
    shape->position = position;
    shape->rotation = rotation;
    shape->size = size;
    m_shapes.push_back(shape);
}

int engine::physics::object_t::body_t::get_id() const {
    return m_id;
}

bool engine::physics::object_t::body_t::is_moving() const {
    return m_moving;
}

const glm::vec3& engine::physics::object_t::body_t::get_rotation_factor() const {
    return m_rotation_factor;
}

const std::vector<std::shared_ptr<engine::physics::object_t::shape_t>>& engine::physics::object_t::body_t::get_shapes() const {
    return m_shapes;
}

const engine::physics::object_t::body_state_t& engine::physics::object_t::body_t::get_state() const {
    return m_state;
}

engine::physics::object_t::body_state_t& engine::physics::object_t::body_t::get_state() {
    return m_state;
}

engine::physics::object_t::object_t(int id, int body_id) :
    game_object_t(id),
    m_body(body_id)
{
}

engine::physics::object_t::~object_t() {
}

const glm::vec3& engine::physics::object_t::get_position() const {
    return m_body.get_state().position;
}

const glm::vec4& engine::physics::object_t::get_rotation() const {
    return m_body.get_state().rotation;
}

void engine::physics::object_t::set_position(const glm::vec3& position) {
    m_body.get_state().position = position;
}

void engine::physics::object_t::set_rotation(const glm::vec4& rotation) {
    m_body.get_state().rotation = rotation;
}

engine::physics::object_t::body_t& engine::physics::object_t::get_body() {
    return m_body;
}
