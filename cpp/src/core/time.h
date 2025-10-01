#pragma once
#include <chrono>

inline std::chrono::local_time<std::chrono::system_clock::duration>
now()
{
    // 1. Get the current system_clock::time_point (UTC)
    const auto sys_now = std::chrono::system_clock::now();

    // 2. Get the current time zone
    const auto& current_zone = *std::chrono::current_zone();

    // 3. Convert the system_clock::time_point to local_time
    //    The to_local() member function of zoned_time performs this conversion.
    return current_zone.to_local(sys_now);
}
