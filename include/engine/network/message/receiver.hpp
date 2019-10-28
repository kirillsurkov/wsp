#pragma once

#include <memory>
#include <unordered_map>
#include <variant>

#include "engine/network/session.hpp"

namespace engine::network::message {
template <class ... Ts>
using builder_variant_t = std::variant<std::shared_ptr<typename Ts::builder_t>...>;

template <class ... Ts>
using builders_map_t = std::map<int, builder_variant_t<Ts...>>;

template <class T_builders_map, class ...>
class receiver_base_t {
protected:
    T_builders_map& m_builders;

public:
    void on_message() = delete;

    receiver_base_t(T_builders_map& builders) :
        m_builders(builders)
    {}
    virtual ~receiver_base_t() {}
    virtual void on_disconnect(std::shared_ptr<session_t>& session) = 0;
};

template <class T_builders_map, class T0, class ... Ts>
class receiver_base_t<T_builders_map, T0, Ts...> : public receiver_base_t<T_builders_map, Ts...> {
public:
    using receiver_base_t<T_builders_map, Ts...>::on_message;

    receiver_base_t(T_builders_map& builders) :
        receiver_base_t<T_builders_map, Ts...>(builders)
    {
    }

    void create() {
        this->m_builders.emplace(T0::id, std::make_shared<typename T0::builder_t>());
    }

    virtual ~receiver_base_t() override {}
    virtual void on_message(std::shared_ptr<session_t>& session, const T0& message) = 0;
};

template <class ... Ts>
class receiver_t : public receiver_base_t<builders_map_t<Ts...>, Ts...> {
private:
    builders_map_t<Ts...> m_builders;

public:
    receiver_t() :
        receiver_base_t<builders_map_t<Ts...>, Ts...>(m_builders)
    {
    }

    virtual ~receiver_t() {
    }

    template <class T_data, class T_messages_receiver> void process_message(int type, T_data& data, std::shared_ptr<engine::network::session_t>& session, std::shared_ptr<T_messages_receiver>& messages_receiver) {
        auto it = m_builders.find(type);
        if (it == m_builders.end()) {
            throw std::runtime_error("Unknown message type " + std::to_string(type));
        }
        std::visit([&](auto&& builder) {
            messages_receiver->on_message(session, *builder->build(data));
        }, it->second);
    }
};
}
