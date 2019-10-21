#include "precompiled.hpp"
#include "game_objects/box.hpp"

box_t::box_t(int id, int body_id, const glm::vec3& size):
    physics_object_t(id, body_id)
{
    m_body.add_shape(physics_object_t::shape_type::box,
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     size);
}

box_t::~box_t() {
}
