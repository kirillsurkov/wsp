#pragma once

#include <vector>
#include <deque>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/pop_front.hpp>

#include "messages_in.hpp"
#include "messages_in/message.hpp"
#include "messages_out.hpp"
#include "messages_out/message.hpp"
#include "binary_writer.hpp"

#define CREATE_MESSAGE_MACRO(r, data, msg) \
    case message::in::type::msg: { \
        BOOST_PP_SEQ_ELEM(1, data) = message::in::BOOST_PP_CAT(msg, _t)::create(BOOST_PP_SEQ_ELEM(0, data)); \
        break; \
    }

#define PROCESS_MESSAGE_MACRO(r, data, msg) \
    case message::in::type::msg: { \
        m_messages_receiver->on_message(this_ptr, std::dynamic_pointer_cast<message::in::BOOST_PP_CAT(msg, _t)>(data)); \
        break; \
    }

#define EXPAND_MESSAGES(macro, type, data, messages) \
    switch (type) { \
        BOOST_PP_SEQ_FOR_EACH(macro, data, messages) \
        default: { \
            throw std::runtime_error("Unknown message type"); \
        } \
    }

#define PROCESS_MESSAGE(msg) EXPAND_MESSAGES(PROCESS_MESSAGE_MACRO, msg->get_type(), (msg), BOOST_PP_SEQ_POP_FRONT(MESSAGES_IN))
#define CREATE_MESSAGE(type, data, out) EXPAND_MESSAGES(CREATE_MESSAGE_MACRO, type, (data)(out), MESSAGES_IN)

class messages_io_t {
public:
    messages_io_t();
    virtual ~messages_io_t();

    virtual void read(binary_reader_t& in, std::vector<std::shared_ptr<message::in::message_t>>& out) const = 0;
    virtual unsigned int write(const std::deque<std::shared_ptr<message::out::message_t>>& in, binary_writer_t& out) const = 0;
};
