#include <vector>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <format>
#include "list_directories.h"
#include "../core/fs.h"
#include <sys/stat.h>

namespace nautix::application {
    namespace fs = std::filesystem;
    namespace chrono = std::chrono;

    std::vector<domain::Directory> ListDirectories::execute(const std::string&& existing_path, const SortOrder order) const {
        std::optional<domain::Directory> directory = domain::Directory::from_existing(existing_path);
        if (directory.has_value()) {
            return execute(*directory, order);
        }
        return {};
    }

    std::vector<domain::Directory> ListDirectories::execute(const domain::Directory& directory, const SortOrder order) const {
        std::vector<domain::Directory> result;
        std::vector<domain::DirectoryInfo> infos;

        for (const fs::directory_entry& entry : fs::directory_iterator(directory.path())) {
            if (!entry.is_directory())
                continue;
            domain::DirectoryInfo info;
            info.path = entry.path();
            info.name = entry.path().filename().string();
            struct stat st{};
            if (stat(info.path.c_str(), &st) == 0) {
                info.size = compute_directory_size(info.path);
                info.modification_time = get_creation_time(st);
                info.creation_time = get_modification_time(st);
                info.access_time = get_access_time(st);
                info.owner_id = st.st_uid;
            }
            infos.push_back(std::move(info));
        }

        std::ranges::sort(infos,
            [order](const domain::DirectoryInfo& a, const domain::DirectoryInfo& b) {
                using enum SortOrder;
                switch (order) {
                    case BySize:
                        return a.size < b.size;
                    case ByOwner:
                        return a.owner_id < b.owner_id;
                    case ByCreationDate:
                        return a.creation_time < b.creation_time;
                    case ByModificationDate:
                        return a.modification_time < b.modification_time;
                    case ByAccessDate:
                        return a.access_time < b.access_time;
                    case ByName:
                    case None:
                    default:
                        return a.name < b.name;
                }
            });

        std::ranges::transform(infos,
            std::back_inserter(result),
            [](domain::DirectoryInfo& info) {
                return domain::Directory(
                    info.path,
                    std::move(info.name),
                    info.size,
                    info.owner_id,
                    info.creation_time,
                    info.modification_time,
                    info.access_time);
            });

        return result;
    }
}
