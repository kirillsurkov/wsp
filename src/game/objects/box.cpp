#include "precompiled.hpp"
#include "game/objects/box.hpp"

box_t::box_t(int id, int body_id, const glm::vec3& size):
    engine::physics::object_t(id, body_id)
{
    m_body.add_shape(engine::physics::object_t::shape_type::box,
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     size);
}

box_t::~box_t() {
}
