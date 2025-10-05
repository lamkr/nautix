#pragma once
#include <chrono>
#include <filesystem>

#include "domain/local_time.h"

nautix::domain::LocalTime
    to_local_time(const std::chrono::system_clock::time_point& time_point);

std::chrono::local_time<std::chrono::duration<long, std::nano>>
    now0();

std::chrono::local_time<std::chrono::duration<long, std::nano>>
    now();
