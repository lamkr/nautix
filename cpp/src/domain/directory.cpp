#include "directory.h"
#include "file.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <pwd.h>
#include <filesystem>
#include <optional>

namespace nautix::domain {

    std::optional<std::filesystem::path> homePath() {
        if (const char* homePath = std::getenv("HOME")) {
            return std::filesystem::path(homePath);
        }

        // Fallback to POSIX systems.
        if (const passwd* pw = getpwuid(getuid())) {
            return std::filesystem::path(pw->pw_dir);
        }
        return std::nullopt;
    }

    const std::optional<Directory>& Directory::home() {
        static const std::optional<Directory> home_dir_singleton = []() -> std::optional<Directory>
        {
            const std::optional path = homePath();
            if (path.has_value()) {
                return Directory(path.value());
            }
            return std::nullopt;
        }();
        return home_dir_singleton;
    }

    const std::filesystem::path& Directory::path() const noexcept {
        return _path;
    }

    const std::vector<File>& Directory::files() const noexcept {
        return _files;
    }

    void Directory::add_file(File file) {
        _files.push_back(std::move(file));
    }

} // namespace nautix::domain
