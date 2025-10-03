#pragma once
#include <chrono>

namespace nautix::domain {

    class LocalTime : public std::chrono::local_time<std::chrono::duration<long, std::nano>> {};

}
