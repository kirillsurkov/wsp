#include "precompiled.hpp"
#include "game/objects/player.hpp"

player_t::player_t(int id, int body_id, int player_id, const std::string& name) :
    engine::physics::object_t(id, body_id),
    m_player_id(player_id),
    m_name(name)
{
    m_body.add_shape(engine::physics::object_t::shape_type::cylinder,
                     glm::vec3(0.0f, 1.0f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     glm::vec3(1.0f, 1.0f, 1.0f));
    m_body.add_shape(engine::physics::object_t::shape_type::sphere,
                     glm::vec3(0.0f, 2.0f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     glm::vec3(1.0f, 1.0f, 1.0f));
    m_body.add_shape(engine::physics::object_t::shape_type::sphere,
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                     glm::vec3(1.0f, 1.0f, 1.0f));
}

player_t::~player_t() {
}

void player_t::set_knows_object(int id) {
    m_known_objects.insert(id);
}

void player_t::remove_knows_object(int id) {
    m_known_objects.erase(id);
}

bool player_t::knows_object(int id) const {
    return m_known_objects.find(id) != m_known_objects.end();
}

int player_t::get_player_id() const {
    return m_player_id;
}

const std::string& player_t::get_name() const {
    return m_name;
}
