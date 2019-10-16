#pragma once

#include "game_object.hpp"

class physics_object_t : public game_object_t {
private:
    int m_body_id;
    glm::vec3 m_position;
    glm::vec4 m_rotation;

public:
    physics_object_t(int id, int body_id);
    virtual ~physics_object_t();

    void set_position(const glm::vec3& position);
    void set_rotation(const glm::vec4& rotation);
    virtual glm::vec3 get_position() const override;
    virtual glm::vec4 get_rotation() const override;

    int get_body_id() const;
};
