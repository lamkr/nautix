#include <chrono>
#include <iostream>
#include <string>
#include <filesystem>

#include "domain/directory.h"
#include "application/list_directories.h"
#include "infra/system_metadata_provider.h"

using namespace nautix::domain;
using namespace nautix::application;

using namespace std::chrono;

int main0() {
    // 1. Create a utc_time point
    // Example: January 1, 2025, 12:00:00 UTC
    constexpr auto utc_tp = sys_days{2025y/1/1} + 12h;

    // 2. Get the local time zone
    // current_zone() returns a pointer to the current system's time zone.

    const time_zone* local_tz = std::chrono::current_zone();//->to_local(utc_tp);

    // 3. Create a zoned_time object
    // This object associates the utc_time with the local time zone.
    const zoned_time zt{local_tz, utc_tp};

    // 4. Access the local_time from the zoned_time
    // The local_time is a time_point in the local_t type,
    // representing the time in the specified time zone.
    local_time<std::chrono::system_clock::duration> local_tp = zt.get_local_time();

    // Optional: Format and print the results
    std::cout << "UTC Time: " << utc_tp << std::endl;
    std::cout << "Local Time: " << zt << std::endl; // zoned_time formats automatically to local time
    std::cout << "Local Time (explicit access): " << local_tp << std::endl;

    return 0;
}

int main() {
    nautix::infra::SystemMetadataProvider provider;
    std::expected<Directory, std::error_code> dir = Directory::home(provider);
    const app::ListDirectories useCase(provider);

    std::cout << "d.p1=" << &dir->path() << ":" << dir->path() << std::endl;
    fs::path path = dir->path();
    std::cout << "p1=" << &path << ":" << path << std::endl;
    path = "upalee";

    std::cout << "d.p1=" << &dir->path() << ":" << dir->path() << std::endl;
    std::cout << "p1=" << &path << ":" << path << std::endl;

    std::string owner_name = dir->get_owner_name().value();
    std::cout << "1=" << &owner_name << ":" << owner_name << std::endl;

    owner_name = "upalele";
    std::cout << "1=" << &owner_name << ":" << owner_name << std::endl;

    owner_name = dir->get_owner_name().value();
    std::cout << "2=" << &owner_name << ":" << owner_name << std::endl;

    owner_name = "upalele";
    std::cout << "2=" << &owner_name << ":" << owner_name << std::endl;

    return 0;
}