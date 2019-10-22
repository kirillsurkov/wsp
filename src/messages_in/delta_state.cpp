#include "precompiled.hpp"
#include "messages_in/delta_state.hpp"

std::shared_ptr<message::in::delta_state_t> message::in::delta_state_t::create(const rapidjson::Value& json) {
    return std::make_shared<delta_state_t>();
}

std::shared_ptr<message::in::delta_state_t> message::in::delta_state_t::create(binary_data_t& data) {
    return std::make_shared<delta_state_t>();
}

message::in::delta_state_t::delta_state_t() :
    message_t(type::delta_state)
{
}

message::in::delta_state_t::~delta_state_t() {
}

std::shared_ptr<player_delta_t> message::in::delta_state_t::get_delta_state() const {
    return nullptr;
}
