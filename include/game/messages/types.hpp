#pragma once

namespace message {
namespace in {
enum type {
    login = 1,
    delta_state,
    chat_local,
    chat_global
};
}
namespace out {
enum class type {
    login = 1,
    chat_local,
    chat_global,
    body_create,
    body_state
};
}
}
