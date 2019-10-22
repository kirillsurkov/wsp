#pragma once

#include "message.hpp"

class player_delta_t;
namespace message::in {
    class delta_state_t : public message_t {
    public:
        static std::shared_ptr<delta_state_t> create(const rapidjson::Value& json);
        static std::shared_ptr<delta_state_t> create(binary_data_t& data);

        delta_state_t();
        virtual ~delta_state_t() override;

        std::shared_ptr<player_delta_t> get_delta_state() const;
    };
}
