#include "precompiled.hpp"
#include "physics_object.hpp"

physics_object_t::body_t::body_t(int id) :
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

physics_object_t::body_t::~body_t() {
}

void physics_object_t::body_t::set_moving(bool moving) {
    m_moving = moving;
}

void physics_object_t::body_t::set_rotation_factor(const glm::vec3& rotation_factor) {
    m_rotation_factor = rotation_factor;
}

void physics_object_t::body_t::add_shape(shape_type type, const glm::vec3& position, const glm::vec4& rotation, const glm::vec3& size) {
    auto shape = std::make_shared<physics_object_t::shape_t>();
    shape->type = type;
    shape->position = position;
    shape->rotation = rotation;
    shape->size = size;
    m_shapes.push_back(shape);
}

int physics_object_t::body_t::get_id() const {
    return m_id;
}

bool physics_object_t::body_t::is_moving() const {
    return m_moving;
}

const glm::vec3& physics_object_t::body_t::get_rotation_factor() const {
    return m_rotation_factor;
}

const std::vector<std::shared_ptr<physics_object_t::shape_t>>& physics_object_t::body_t::get_shapes() const {
    return m_shapes;
}

const physics_object_t::body_state_t& physics_object_t::body_t::get_state() const {
    return m_state;
}

physics_object_t::body_state_t& physics_object_t::body_t::get_state() {
    return m_state;
}

physics_object_t::physics_object_t(int id, int body_id) :
    game_object_t(id),
    m_body(body_id)
{
}

physics_object_t::~physics_object_t() {
}

const glm::vec3& physics_object_t::get_position() const {
    return m_body.get_state().position;
}

const glm::vec4& physics_object_t::get_rotation() const {
    return m_body.get_state().rotation;
}

void physics_object_t::set_position(const glm::vec3& position) {
    m_body.get_state().position = position;
}

void physics_object_t::set_rotation(const glm::vec4& rotation) {
    m_body.get_state().rotation = rotation;
}

physics_object_t::body_t& physics_object_t::get_body() {
    return m_body;
}
