#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <system_error>
#include <format>
#include <sys/stat.h>
#include "list_directories.h"
#include <sys/syscall.h> // For syscall
#include <fcntl.h>       // For AT_FDCWD
#include <unistd.h>

// Define __NR_statx if not already defined (might be needed for older headers)
#ifndef __NR_statx
#if defined(__x86_64__)
#define __NR_statx 332
#elif defined(__i386__)
#define __NR_statx 383
#elif defined(__aarch64__)
#define __NR_statx 291
#else
#error "Unsupported architecture for statx system call"
#endif
#endif

namespace nautix::application {
    using namespace nautix::domain;
    namespace fs = std::filesystem;

    static std::chrono::system_clock::time_point get_modification_time(const fs::path&);

    static std::chrono::system_clock::time_point get_creation_time_legacy(const fs::path& path) {
        struct stat stat_dir;
        if (stat(path.c_str(), &stat_dir) == 0) {
            return std::chrono::system_clock::from_time_t(stat_dir.st_ctim.tv_sec) +
                   std::chrono::nanoseconds(stat_dir.st_ctim.tv_nsec);
        }
        return get_modification_time(path);
    }

    // Function to get the creation time of a directory using statx
    std::chrono::system_clock::time_point get_creation_time(const std::filesystem::path& path) {
        struct statx stat_dir;
        const int descriptor = AT_FDCWD;
        const unsigned int flags = AT_SYMLINK_NOFOLLOW; // Don't follow symlinks
        const unsigned int mask = STATX_BTIME; // Request birth time

        if (syscall(__NR_statx, descriptor, path.c_str(), flags, mask, &stat_dir) == 0) {
            // Convert timespec to std::chrono::system_clock::time_point
            return std::chrono::system_clock::from_time_t(stat_dir.stx_btime.tv_sec) +
                   std::chrono::nanoseconds(stat_dir.stx_btime.tv_nsec);
        }
        return get_creation_time_legacy(path);
    }

    static std::chrono::system_clock::time_point get_modification_time(const fs::path& path) {
        return std::chrono::clock_cast<std::chrono::system_clock>(
            std::filesystem::last_write_time(path) );
    }

    static std::chrono::system_clock::time_point get_access_time(const fs::path& path) {
        struct stat stat_dir;
        if (stat(path.c_str(), &stat_dir) == 0) {
            return std::chrono::system_clock::from_time_t(stat_dir.st_atim.tv_sec) +
                   std::chrono::nanoseconds(stat_dir.st_atim.tv_nsec);
        }
        return std::chrono::system_clock::from_time_t(time_t {});
    }

    static __uid_t get_owner(const fs::path& p) {
        struct stat statA{};
        if (stat(p.c_str(), &statA) == 0) {
            return statA.st_uid;
        }
        return 0;
    }

    static std::string getTimeISO8601(std::chrono::system_clock::time_point time_point) {
        // Get milliseconds
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            time_point.time_since_epoch()
        ) % 1000;

        // Convert to time_t for gmtime
        const std::time_t the_time_t = std::chrono::system_clock::to_time_t(time_point);
        const std::tm* tm_gmt = std::gmtime(&the_time_t); // Get UTC time

        if (tm_gmt == nullptr) {
            return ""; // Error occurred with gmtime
        }

        // Format the time
        std::ostringstream oss;
        oss << std::put_time(tm_gmt, "%Y-%m-%dT%H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count() << "Z"; // Add milliseconds and 'Z' for UTC

        return oss.str();
    }

    static void showInfo(const fs::path& path, int what) {
        std::chrono::system_clock::time_point creationTime = get_creation_time(path);
        //std::time_t c_time = std::chrono::system_clock::to_time_t(creationTime);

        std::chrono::system_clock::time_point modificationTime = get_modification_time(path);
        //std::time_t m_time = std::chrono::system_clock::to_time_t(modificationTime);

        std::chrono::system_clock::time_point accessTime = get_access_time(path);
        //std::time_t a_time = std::chrono::system_clock::to_time_t(accessTime);

        __uid_t owner = get_owner(path);

        std::chrono::system_clock::time_point theTime;
        if (what == 'C') {
            theTime = creationTime;
        }
        else if (what == 'M') {
            theTime = modificationTime;
        }
        else if (what == 'A') {
            theTime = accessTime;
        }
        std::cout << "\t" << path
            << " " << static_cast<char>(what) << ": " << getTimeISO8601(theTime)
            << " O:" << owner
            << std::endl;
    }

    static void show(std::vector<Directory> vect,int what) {
        std::cout << "result: " << std::endl;
        for ( unsigned long x = 0; x < vect.size(); ++x ) {
            auto dir = vect[x];
            showInfo(dir.path(), what);
        }
    }

    static void show(std::vector<fs::directory_entry> vect,int what) {
        std::cout << "dirs: " << std::endl;
        for ( unsigned long x = 0; x < vect.size(); ++x ) {
            auto dir = vect[x];
            showInfo(dir.path(), what);
        }
    }


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
                            std::chrono::system_clock::time_point timeA = get_creation_time(a.path());
                            std::chrono::system_clock::time_point timeB = get_creation_time(b.path());
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
} // namespace nautix::application
