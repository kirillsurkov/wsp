#include "precompiled.hpp"
#include "game/messages/client/delta_state.hpp"

message::in::delta_state_t::builder_t::~builder_t() {
}

std::shared_ptr<message::in::delta_state_t> message::in::delta_state_t::builder_t::build(const rapidjson::Value& json) const {
    return std::make_shared<delta_state_t>();
}

std::shared_ptr<message::in::delta_state_t> message::in::delta_state_t::builder_t::build(binary_reader_t& reader) const {
    return std::make_shared<delta_state_t>();
}

message::in::delta_state_t::delta_state_t() {
}

message::in::delta_state_t::~delta_state_t() {
}
