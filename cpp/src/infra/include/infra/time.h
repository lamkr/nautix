#pragma once
#include <chrono>
#include <filesystem>

#include "domain/local_time.h"

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    to_local_time(const std::chrono::system_clock::time_point& time_point);

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
std::chrono::local_time<std::chrono::duration<long, std::nano>>
    now0();

// std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::ratio<1, 1000000000>>>
std::chrono::local_time<std::chrono::duration<long, std::nano>>
    now();
