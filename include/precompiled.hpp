#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#include <deque>
#include <unordered_map>
#include <unordered_set>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include <rapidjson/document.h>

#include "log.hpp"

#define LOG log_t()
