#pragma once

#include "physics_object.hpp"

class physics_t {
public:
    physics_t();
    virtual ~physics_t();

    virtual void add_body(const physics_object_t::body_t& body) = 0;
    virtual void destroy_body(int body_id) = 0;

    virtual void update_body_state(physics_object_t::body_t& body) = 0;

    virtual void update() = 0;
};
