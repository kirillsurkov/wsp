#include "precompiled.hpp"
#include "object.hpp"

object_t::object_t(int id) :
	m_id(id)
{
}

object_t::~object_t() {
}

int object_t::get_id() const {
	return m_id;
}
