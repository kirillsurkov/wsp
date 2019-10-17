#pragma once

#include <string>

#include "physics_object.hpp"

class player_t : public physics_object_t {
private:
    int m_player_id;
    std::string m_name;

public:
    player_t(int id, int body_id, int player_id, const std::string& name);
    virtual ~player_t() override;

    int get_player_id() const;
    const std::string& get_name() const;
};
