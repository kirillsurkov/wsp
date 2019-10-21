#include "precompiled.hpp"
#include "v8_platform.hpp"
#include "physics_oimo.hpp"
#include "listener.hpp"
#include "core.hpp"

#define PORT 34851
#define THREADS 4
#define TIME_STEP (1.0f / 60.0f)

int main() {
    v8_platform_t v8_platform;
    auto physics = std::make_shared<physics_oimo_t>(v8_platform.create_isolate(), TIME_STEP);
    core_t core(physics, TIME_STEP);
    boost::asio::io_context io{THREADS};
    std::make_shared<listener_t>(io, core, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("0.0.0.0"), PORT))->run();
    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back([&io]() {
            io.run();
        });
    }
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    return 0;
}
