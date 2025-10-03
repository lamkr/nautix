#pragma once
#include <chrono>

namespace nautix::domain {

    class LocalTime : public std::chrono::local_time<std::chrono::duration<long, std::nano>> {};

    class LocalTime2 {
        std::chrono::local_time<std::chrono::duration<long, std::nano>> local_time_{};

    public:
        explicit LocalTime2(std::chrono::local_time<std::chrono::duration<long, std::nano>>&& local_time)
            //: local_time_(std::forward<std::chrono::local_time<std::chrono::system_clock::duration>>(local_time))
            : local_time_(local_time)
        {}


        auto operator<=>(const LocalTime2& local_time) const = default;
    };
}
