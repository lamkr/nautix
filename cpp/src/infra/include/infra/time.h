#pragma once
#include <chrono>

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
std::chrono::local_time<std::chrono::duration<long, std::nano>>
    now0();

// std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::ratio<1, 1000000000>>>
std::chrono::local_time<std::chrono::duration<long, std::nano>>
    now();
