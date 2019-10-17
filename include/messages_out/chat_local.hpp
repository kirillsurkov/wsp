#pragma once

#include "message.hpp"

namespace message::out {
    class chat_local_t : public message_t {
    private:
        int m_sender_id;
        std::string m_text;

    protected:
        virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

    public:
        chat_local_t(int frame, int sender_id, const std::string& text);
        virtual ~chat_local_t() override;
    };
}
