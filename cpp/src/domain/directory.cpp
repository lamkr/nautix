#include "directory.h"
#include "file.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <pwd.h>
#include <filesystem>
#include <optional>

#include "fs.h"

namespace nautix::domain {

    namespace fs = std::filesystem;
    
    std::optional<fs::path> homePath() {
        if (const char* homePath = std::getenv("HOME")) {
            return fs::path(homePath);
        }

        // Fallback to POSIX systems.
        if (const passwd* pw = getpwuid(getuid())) {
            return fs::path(pw->pw_dir);
        }
        return std::nullopt;
    }

    const std::optional<Directory>& Directory::home() {
        static const std::optional<Directory> home_dir_singleton = []() -> std::optional<Directory> {
            const std::optional path = homePath();
            if (path.has_value()) {
                return Directory(path.value());
            }
            return std::nullopt;
        }();
        return home_dir_singleton;
    }

    const std::optional<Directory>& Directory::temp() {
        static const std::optional<Directory> temp_dir_singleton = []() -> std::optional<Directory> {
            return Directory(fs::temp_directory_path());
        }();
        return temp_dir_singleton;
    }

    [[nodiscard]] const Directory& Directory::from_directory_entry(fs::directory_entry& directory_entry) {
        return Directory(
            directory_entry.path(),
            to_local_time(get_creation_time(directory_entry.path())),
            to_local_time(get_modification_time(directory_entry.path())),
            to_local_time(get_access_time(directory_entry.path())),
            get_owner(directory_entry.path())
        );
    }

    const fs::path& Directory::path() const noexcept {
        return _path;
    }

    const std::vector<File>& Directory::files() const noexcept {
        return _files;
    }

    void Directory::add_file(File file) {
        _files.push_back(std::move(file));
    }

}
