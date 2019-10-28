#pragma once

#include "engine/network/message/client.hpp"

#include "game/messages/types.hpp"

namespace message::in {
    class chat_local_t : public engine::network::message::client_t<type::chat_local, chat_local_t> {
    public:
        class builder_t : public chat_local_t::type::builder_t {
        public:
            virtual ~builder_t() override;
            virtual std::shared_ptr<chat_local_t> build(const rapidjson::Value& json) const override;
            virtual std::shared_ptr<chat_local_t> build(binary_reader_t& reader) const override;
        };

    private:
        std::string m_text;

    public:
        chat_local_t(const std::string& text);
        virtual ~chat_local_t() override;

        const std::string& get_text() const;
    };
}
