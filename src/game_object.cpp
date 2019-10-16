#include "precompiled.hpp"
#include "game_object.hpp"

game_object_t::game_object_t(int id) :
    m_id(id)
{
}

game_object_t::~game_object_t() {
}

int game_object_t::get_id() const {
    return m_id;
}
