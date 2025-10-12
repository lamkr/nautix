#pragma once
#include <string>

namespace nautix::domain {
    namespace {
        constexpr auto UNKNOWN = "<unknown>";
    }

    class Owner {
        uid_t id_;
        std::string name_{};

    public:
        Owner() : Owner(static_cast<uid_t>(-1), UNKNOWN) {}

        explicit Owner(const uid_t id, std::string name)
            : id_(id)
            , name_(std::move(name))//std::forward<std::chrono::local_time<std::chrono::system_clock::duration>>(local_time))
        {}

        [[nodiscard]] const std::string& name() const { return name_; }
        [[nodiscard]] uid_t id() const { return id_; }
        [[nodiscard]] bool is_unknown() const { return name_==UNKNOWN; }
    };

    static const auto unknown_owner = Owner();
}
