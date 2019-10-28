#include "precompiled.hpp"
#include "engine/game_object.hpp"

game_object_t::game_object_t(int id) :
    m_id(id)
{
}

game_object_t::~game_object_t() {
}

void game_object_t::set_known_by(int id) {
    m_known_by.insert(id);
}

void game_object_t::remove_known_by(int id) {
    m_known_by.erase(id);
}

bool game_object_t::known_by(int id) const {
    return m_known_by.find(id) != m_known_by.end();
}

int game_object_t::get_id() const {
    return m_id;
}
