export module nautix.domain:owner;

#include <string>
#include <sys/types.h>

export class Owner {
    uid_t id_{};
    std::string name_{};

public:
    Owner() = default;
    explicit Owner(const uid_t id, std::string&& name)
        : id_(id)
        , name_(name)//std::forward<std::chrono::local_time<std::chrono::system_clock::duration>>(local_time))
    {}
};
