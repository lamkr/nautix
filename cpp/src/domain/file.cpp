#include "file.h"
#include <filesystem>

namespace nautix::domain {

    File::File(std::string path, std::uintmax_t size,
               std::chrono::system_clock::time_point modified)
        : _path(std::move(path)), _size(size), _modified(modified) {}

    const std::string& File::path() const noexcept {
        return _path;
    }

    std::string File::name() const {
        return std::filesystem::path(_path).filename().string();
    }

    std::uintmax_t File::size() const noexcept {
        return _size;
    }

    std::chrono::system_clock::time_point File::last_modified() const noexcept {
        return _modified;
    }

} // namespace nautix::domain
