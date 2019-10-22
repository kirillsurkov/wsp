#pragma once

#include "message.hpp"

namespace message::in {
    class login_t : public message_t {
    private:
        std::string m_name;

    public:
        static std::shared_ptr<login_t> create(const rapidjson::Value& json);
        static std::shared_ptr<login_t> create(binary_data_t& data);

        login_t(const std::string& name);
        virtual ~login_t() override;

        const std::string& get_name() const;
    };
}
