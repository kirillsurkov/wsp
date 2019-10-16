#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class game_object_t {
private:
    int m_id;

public:
    game_object_t(int id);
    virtual ~game_object_t();

    virtual glm::vec3 get_position() const = 0;
    virtual glm::vec4 get_rotation() const = 0;

    int get_id() const;
};
