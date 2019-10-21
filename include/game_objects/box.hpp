#pragma once

#include <glm/vec3.hpp>

#include "physics_object.hpp"

class box_t : public physics_object_t {
public:
    box_t(int id, int body_id, const glm::vec3& size);
    virtual ~box_t() override;
};
