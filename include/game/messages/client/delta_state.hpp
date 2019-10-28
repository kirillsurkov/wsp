#pragma once

#include "engine/network/message/client.hpp"

#include "game/messages/types.hpp"

namespace message::in {
    class delta_state_t : public engine::network::message::client_t<type::delta_state, delta_state_t> {
    public:
        class builder_t : public delta_state_t::type::builder_t {
        public:
            virtual ~builder_t() override;
            virtual std::shared_ptr<delta_state_t> build(const rapidjson::Value& json) const override;
            virtual std::shared_ptr<delta_state_t> build(binary_reader_t& reader) const override;
        };

    public:
        delta_state_t();
        virtual ~delta_state_t() override;
    };
}
