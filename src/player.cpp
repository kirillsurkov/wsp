#include "precompiled.hpp"
#include "player.hpp"

player_t::player_t(int id, int body_id, int player_id, const std::string& name) :
	physics_object_t(id, body_id),
	m_player_id(player_id),
    m_name(name)
{
}

player_t::~player_t() {
}

int player_t::get_player_id() const {
	return m_player_id;
}

const std::string& player_t::get_name() const {
    return m_name;
}
