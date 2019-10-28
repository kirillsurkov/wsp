#pragma once

#include <unordered_set>

class id_manager_t {
private:
    int m_used;
    std::unordered_set<int> m_unused;

public:
    id_manager_t();
    ~id_manager_t();

    int next_id();
    void free_id(int id);
};
