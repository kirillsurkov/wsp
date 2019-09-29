#include "precompiled.hpp"
#include "id_manager.hpp"

id_manager_t::id_manager_t() :
    m_used(0)
{
}

id_manager_t::~id_manager_t() {
}

int id_manager_t::next_id() {
    int id;
    if (m_unused.size()) {
        id = *m_unused.begin();
        m_unused.erase(id);
    } else {
        id = m_used + 1;
    }
    m_used++;
    return id;
}

void id_manager_t::free_id(int id) {
    m_used--;
    m_unused.insert(id);
}
