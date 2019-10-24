#include "precompiled.hpp"
#include "v8_platform.hpp"
#include "physics_engines/physics_oimo.hpp"
#include "core.hpp"
#include "messages_io/io_binary.hpp"
#include "messages_io/io_json.hpp"
#include "network.hpp"

#define PORT 34851
#define THREADS 4
#define TIME_STEP (1.0f / 60.0f)

int main() {
    v8_platform_t v8_platform;
    auto physics = std::make_shared<physics_oimo_t>(v8_platform.create_isolate(), TIME_STEP);
    auto core = std::make_shared<core_t>(physics, TIME_STEP);
    io_binary_t messages_io;
    auto network = std::make_shared<network_t>(core, PORT, THREADS, messages_io);
    network->run();
    network->wait();
    return 0;
}
