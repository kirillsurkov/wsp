#include "precompiled.hpp"
#include "messages_in/delta_state.hpp"

message::in::delta_state_t::delta_state_t(const rapidjson::Value& json) :
    message_t(type::delta_state)
{
}

message::in::delta_state_t::~delta_state_t() {
}

std::shared_ptr<player_delta_t> message::in::delta_state_t::get_delta_state() const {
	return nullptr;
}
