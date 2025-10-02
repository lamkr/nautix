module;

#include <chrono>

export module nautix.domain:local_time;

export class LocalTime {
    std::chrono::local_time<std::chrono::system_clock::duration> local_time_{};

    explicit LocalTime(std::chrono::local_time<std::chrono::system_clock::duration> local_time)
        : local_time_(std::forward<std::chrono::local_time<std::chrono::system_clock::duration>>(local_time))
    {}

public:
    LocalTime() = default;

    static LocalTime from_local_time(std::chrono::local_time<std::chrono::system_clock::duration>&& local_time) {
        return LocalTime(local_time);
    }
};
