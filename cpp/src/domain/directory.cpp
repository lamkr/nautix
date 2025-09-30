#include <sys/stat.h>
#include "../core/std.h"
#include "../core/fs.h"

namespace nautix::domain {

    STATIC std::optional<Directory> Directory::from_existing(const std::string& existing_path) {
        if (!fs::exists(existing_path) || !fs::is_directory(existing_path))
            return {};

        const fs::path path = existing_path;
        struct stat st{};
        if (stat(path.c_str(), &st))
            return {};

        return domain::Directory(
            path,
            path.filename().string(),
            compute_directory_size(path),
            st.st_uid,
            get_creation_time(st),
            get_modification_time(st),
            get_access_time(st)
        );
    }

    STATIC std::optional<Directory> Directory::home() {
        static const auto home_dir_singleton = []() -> std::optional<Directory> {
            if (const auto path = get_home_path()) {
                return from_existing(path->string());
            }
            return std::nullopt;
        }();
        return home_dir_singleton;
    }

    STATIC std::optional<Directory> Directory::temp() {
        static const auto temp_dir_singleton = []() -> std::optional<Directory> {
            const fs::path temp_path = fs::temp_directory_path();
            return from_existing(temp_path.string());
        }();
        return temp_dir_singleton;
    }

    std::string Directory::get_owner_name() const noexcept {
        return ::get_owner_name(owner_id_);
    }

    const std::vector<File>& Directory::files() const noexcept {
        return files_;
    }

    void Directory::add_file(File file) {
        files_.push_back(std::move(file));
    }

}
