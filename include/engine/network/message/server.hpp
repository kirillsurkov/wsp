#pragma once

#include <string>
#include <rapidjson/writer.h>

#include "engine/utils/binary_writer.hpp"

namespace engine::network::message {
    class server_t {
    private:
        int m_type;
        int m_frame;

    protected:
        virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;
        virtual void write_data(binary_writer_t& writer) const = 0;

    public:
        server_t(int type, int frame);
        virtual ~server_t();

        void write(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
        void write(binary_writer_t& writer) const;
    };
}
