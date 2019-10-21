#pragma once

#include <string>
#include <unordered_set>

#include "physics_object.hpp"

class player_t : public physics_object_t {
private:
    int m_player_id;
    std::string m_name;
    std::unordered_set<int> m_known_objects;

public:
    player_t(int id, int body_id, int player_id, const std::string& name);
    virtual ~player_t() override;

    void set_knows_object(int id);
    void remove_knows_object(int id);
    bool knows_object(int id) const;

    int get_player_id() const;
    const std::string& get_name() const;
};
