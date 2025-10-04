#pragma once
#include <chrono>

namespace nautix::domain {

    typedef std::chrono::local_time<std::chrono::duration<long, std::nano>> LocalTime;

}
