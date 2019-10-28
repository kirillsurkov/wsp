#pragma once

#include <glm/vec3.hpp>

#include "engine/physics/physics_object.hpp"

class box_t : public engine::physics::object_t {
public:
    box_t(int id, int body_id, const glm::vec3& size);
    virtual ~box_t() override;
};
