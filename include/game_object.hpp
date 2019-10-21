#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <unordered_set>

class game_object_t {
private:
    int m_id;
    std::unordered_set<int> m_known_by;

public:
    game_object_t(int id);
    virtual ~game_object_t();

    virtual const glm::vec3& get_position() const = 0;
    virtual const glm::vec4& get_rotation() const = 0;

    void set_known_by(int id);
    void remove_known_by(int id);
    bool known_by(int id) const;

    int get_id() const;
};
