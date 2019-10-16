#include "precompiled.hpp"
#include "physics_object.hpp"

physics_object_t::physics_object_t(int id, int body_id) :
    game_object_t(id),
    m_body_id(body_id)
{
}

physics_object_t::~physics_object_t() {
}

void physics_object_t::set_position(const glm::vec3& position) {
    m_position = position;
}

void physics_object_t::set_rotation(const glm::vec4& rotation) {
    m_rotation = rotation;
}

glm::vec3 physics_object_t::get_position() const {
    return m_position;
}

glm::vec4 physics_object_t::get_rotation() const {
    return m_rotation;
}

int physics_object_t::get_body_id() const {
    return m_body_id;
}

