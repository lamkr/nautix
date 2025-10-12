#pragma once
#include <chrono>

namespace nautix::domain {
    using LocalTime = std::chrono::local_time<std::chrono::system_clock::duration>;
}
