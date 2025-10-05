#pragma once
#include <string>

namespace nautix::domain {

    const auto UNKNOWN_OWNER_NAME = "<unknown>";

    class Owner {
        uid_t id_;
        std::string name_{};

    public:
        Owner() : Owner(static_cast<uid_t>(-1), UNKNOWN_OWNER_NAME) {}

        explicit Owner(const uid_t id, std::string name)
            : id_(id)
            , name_(std::move(name))//std::forward<std::chrono::local_time<std::chrono::system_clock::duration>>(local_time))
        {}

        [[nodiscard]] const std::string& name() const { return name_; }
        [[nodiscard]] uid_t id() const { return id_; }
    };
}