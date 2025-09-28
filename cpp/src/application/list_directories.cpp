#include <vector>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <system_error>
#include <format>
#include "list_directories.h"
#include "fs.h"

namespace nautix::application {
    using namespace nautix::domain;
    namespace fs = std::filesystem;
    namespace chrono = std::chrono;

    std::vector<Directory> ListDirectories::execute(const Directory& root, SortOrder order) const {
        std::vector<fs::directory_entry> dirs;
        std::vector<Directory> result;
        int what = 0;
        try {
            if (!fs::exists(root.path()) || !fs::is_directory(root.path())) {
                return result;
            }

            for (const fs::directory_entry& entry : fs::directory_iterator(root.path())) {
                if (entry.is_directory()) {
                    dirs.emplace_back(entry);
                }
            }

            switch (order) {
                    using enum SortOrder;

                case ByName:
                    std::ranges::sort(dirs,
                        [](const fs::directory_entry& a, const fs::directory_entry& b) {
                            return a.path() < b.path();
                        });
                    break;

                case ByCreationDate:
                    what = 'C';
                    std::ranges::sort(dirs,
                        [](const fs::directory_entry& a, const fs::directory_entry& b) {
                            chrono::system_clock::time_point timeA = get_creation_time(a.path());
                            chrono::system_clock::time_point timeB = get_creation_time(b.path());
                            return timeA < timeB;
                        });
                    break;

                case ByModificationDate:
                    what = 'M';
                    std::ranges::sort(dirs,
                        [](const fs::directory_entry& a, const fs::directory_entry& b) {
                            const auto timeA = get_modification_time(a);
                            const auto timeB = get_modification_time(b);
                            return timeA < timeB;
                        });
                    break;

                case ByAccessDate:
                    what = 'A';
                    std::ranges::sort(dirs,
                        [](const fs::directory_entry& a, const fs::directory_entry& b) {
                            const auto timeA = get_access_time(a);
                            const auto timeB = get_access_time(b);
                            return timeA < timeB;
                        });
                    break;

                case BySize:
                    std::ranges::sort(dirs,
                        [](const fs::directory_entry& a, const fs::directory_entry& b) {
                            std::uintmax_t sizeA = 0, sizeB = 0;
                            std::error_code errorA, errorB;
                            sizeA = fs::file_size(a, errorA);
                            sizeB = fs::file_size(b, errorB);
                            return !(errorA && errorB) ? sizeA < sizeB : false;
                        });
                    break;

                case ByOwner:
                    std::ranges::sort(dirs,
                        [](const fs::directory_entry& a, const fs::directory_entry& b) {
                            struct stat statA{}, statB{};
                            if (stat(a.path().c_str(), &statA) != 0)
                                return false;
                            if (stat(b.path().c_str(), &statB) != 0)
                                return true;
                            return statA.st_uid < statB.st_uid;
                        });
                    break;

                case None:
                default:
                    break;
            }
        }
        catch (const fs::filesystem_error&) {
            // Em TDD: ainda não tratamos erros específicos
        }

        show(dirs, what);

        for (const fs::directory_entry& entry : dirs) {
            result.push_back(Directory(entry));
        }

        show(result, what);

        return result;
    }
}
