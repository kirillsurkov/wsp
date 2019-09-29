#pragma once

#include "message.hpp"

class player_delta_t;
namespace message::in {
    class delta_state_t : public message_t {
    private:
        

    public:
        delta_state_t(const rapidjson::Value& json);
        virtual ~delta_state_t();

        std::shared_ptr<player_delta_t> get_delta_state() const;
    };
}
