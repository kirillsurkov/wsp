#include "precompiled.hpp"
#include "v8_platform.hpp"
#include "physics_oimo.hpp"
#include "core.hpp"
#include "network.hpp"

#define PORT 34851
#define THREADS 4
#define TIME_STEP (1.0f / 60.0f)

int main() {
    v8_platform_t v8_platform;
    auto physics = std::make_shared<physics_oimo_t>(v8_platform.create_isolate(), TIME_STEP);
    auto core = std::make_shared<core_t>(physics, TIME_STEP);
    auto network = std::make_shared<network_t>(core, PORT, THREADS, network_t::protocol::json);
    network->run();
    network->wait();
    return 0;
}
