#pragma once

#include "messages_out/message.hpp"

namespace message::out {
    class login_t : public message_t {
    private:
        int m_frame;

    protected:
        virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

    public:
        login_t(int frame);
        virtual ~login_t() override;
    };
}
