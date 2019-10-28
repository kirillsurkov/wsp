#pragma once

#include "engine/network/message/client.hpp"

#include "game/messages/types.hpp"

namespace message::in {
    class chat_global_t : public engine::network::message::client_t<type::chat_global, chat_global_t> {
    public:
        class builder_t : public chat_global_t::type::builder_t {
        public:
            virtual ~builder_t() override;
            virtual std::shared_ptr<chat_global_t> build(const rapidjson::Value& json) const override;
            virtual std::shared_ptr<chat_global_t> build(binary_reader_t& reader) const override;
        };

    private:
        std::string m_text;

    public:
        chat_global_t(const std::string& text);
        virtual ~chat_global_t() override;

        const std::string& get_text() const;
    };
}
