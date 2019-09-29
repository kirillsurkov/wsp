#include "precompiled.hpp"
#include "player.hpp"
#include "session.hpp"

player_t::player_t(std::shared_ptr<session_t> session, const std::string& name) :
	object_t(session->get_id()),
    m_session(session),
    m_name(name)
{
}

player_t::~player_t() {
}

std::shared_ptr<session_t> player_t::get_session() {
    return m_session;
}

const std::string& player_t::get_name() const {
    return m_name;
}
