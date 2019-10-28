#include "precompiled.hpp"
#include "engine/network/session.hpp"

engine::network::session_t::session_t() :
    m_player(0)
{
}

engine::network::session_t::~session_t() {
}

void engine::network::session_t::set_player(int id) {
    m_player = id;
}

int engine::network::session_t::get_player() const {
    return m_player;
}
