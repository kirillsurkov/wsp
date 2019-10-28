#include "precompiled.hpp"
#include "engine/physics/physics.hpp"

engine::physics_t::physics_t() {
    LOG << "Physics engine started";
}

engine::physics_t::~physics_t() {
    LOG << "Physics engine stopped";
}
