#pragma once

#include "object.hpp"

class session_t;
class player_t : public object_t {
private:
    std::shared_ptr<session_t> m_session;
    std::string m_name;

public:
    player_t(std::shared_ptr<session_t> session, const std::string& name);
    ~player_t();

    std::shared_ptr<session_t> get_session();
    const std::string& get_name() const;
};
