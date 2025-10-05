#include "infra/time.h"

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    to_local_time(const std::chrono::system_clock::time_point time_point)
{
    // Get the local time zone.
    const std::chrono::time_zone* local_tz = std::chrono::current_zone();
    // Create a zoned_time object.
    // This object associates the utc_time with the local time zone.
    const std::chrono::zoned_time zt{local_tz, time_point};
    // Access the local_time from the zoned_time
    // The local_time is a time_point in the local_t type,
    // representing the time in the specified time zone.
    const std::chrono::local_time<std::chrono::duration<long, std::nano>> local_tm = zt.get_local_time();
    return nautix::domain::LocalTime {local_tm};
}

// Função helper para converter o tempo do sistema de arquivos para o nosso tipo LocalTime
nautix::domain::LocalTime to_local_time2(const std::filesystem::file_time_type& ftime) {
    const auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    return std::chrono::current_zone()->to_local(sctp);
}


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
