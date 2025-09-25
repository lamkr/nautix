#pragma once
#include <string>
#include <chrono>

namespace nautix::domain {

    class File {
    public:
        File(std::string path, std::uintmax_t size,
             std::chrono::system_clock::time_point modified);

        [[nodiscard]] const std::string& path() const noexcept;
        [[nodiscard]] std::string name() const;
        [[nodiscard]] std::uintmax_t size() const noexcept;
        [[nodiscard]] std::chrono::system_clock::time_point last_modified() const noexcept;

    private:
        std::string _path;
        std::uintmax_t _size;
        std::chrono::system_clock::time_point _modified;
    };

} // namespace nautix::domain
