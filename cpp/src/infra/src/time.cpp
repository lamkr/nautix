#include "include/time.h"

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
std::chrono::local_time<std::chrono::duration<long, std::nano>>
now0()
{
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    const std::chrono::time_zone* tz_local = std::chrono::current_zone();

    /*const std::chrono::zoned_time<std::chrono::duration<long, std::nano>>
        zt_local0 = std::chrono::zoned_time { tz_local, now };
    const std::chrono::zoned_time
        zt_local1 = std::chrono::zoned_time { tz_local, now };
    const std::chrono::zoned_time<std::chrono::duration<long, std::nano>>
        zt_local2 { tz_local, now };*/
    const std::chrono::zoned_time
        zt_local0 { tz_local, now };

    return zt_local0.get_local_time();
}


std::chrono::local_time<std::chrono::duration<long, std::nano>>
// std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::ratio<1, 1000000000>>>
now()
{
    const std::chrono::zoned_time<std::chrono::duration<long, std::nano>> local
        = std::chrono::zoned_time {
            std::chrono::current_zone(),
            std::chrono::system_clock::now()
        };
    return local.get_local_time();

}
