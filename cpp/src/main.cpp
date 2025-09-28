#include <chrono>
#include <iostream>
#include <string>
#include <filesystem>
#include <utility>

#include "directory.h"
#include "list_directories.h"

using namespace nautix::application;
using namespace nautix::domain;

// --- Exemplo de Uso Abrangente ---
int main2() {
    // 1. De um literal de ‘string’ (const char*). Nenhuma ‘std::string’ temporária é criada.
    Directory d1("/home/user/music");
    std::cout << "1. De const char*: " << d1.path() << std::endl;

    // 2. De uma std::string (lvalue). Ocorrerá uma cópia.
    std::string downloads_str = "/home/user/downloads";
    Directory d2(downloads_str);
    std::cout << "2. De std::string lvalue (copiado): " << d2.path() << std::endl;

    // 3. De uma std::string (rvalue), movendo explicitamente. Ocorrerá um movimento.
    Directory d3(std::move(downloads_str));
    std::cout << "3. De std::string rvalue (movido): " << d3.path() << std::endl;
    std::cout << "   -> downloads_str original agora: '" << downloads_str << "'\n";

    // 4. De um std::filesystem::path (lvalue). Cópia.
    std::filesystem::path pictures_path = "/home/user/pictures";
    Directory d4(pictures_path);
    std::cout << "4. De path lvalue (copiado): " << d4.path() << std::endl;

    // 5. De um std::filesystem::path (rvalue). Movimento.
    Directory d5(std::filesystem::path("/home/user/videos"));
    std::cout << "5. De path rvalue (movido): " << d5.path() << std::endl;

    // 6.
    Directory d6(d1);
    std::cout << "6. De path rvalue (movido): " << d6.path() << std::endl;

    // 7.
    Directory d7(std::move(d1));
    std::cout << "7. De path rvalue (movido): " << d6.path() << std::endl;

    std::cout << "1. De const char*: " << d1.path() << std::endl;

    return 0;
}

int main1()
{
    nautix::application::ListDirectories useCase;

    auto subdirs = useCase.execute(nautix::domain::Directory::home().value());

    for (const auto& dir : subdirs)
    {
        std::cout << "📂 " << dir.path() << "\n";
    }
    return 0;
}

using namespace std::chrono;

int main() {
    // 1. Create a utc_time point
    // Example: January 1, 2025, 12:00:00 UTC
    auto utc_tp = std::chrono::sys_days{2025y/1/1} + 12h;

    // 2. Get the local time zone
    // current_zone() returns a pointer to the current system's time zone.

    const std::chrono::time_zone* local_tz = std::chrono::current_zone();//->to_local(utc_tp);

    // 3. Create a zoned_time object
    // This object associates the utc_time with the local time zone.
    std::chrono::zoned_time zt{local_tz, utc_tp};

    // 4. Access the local_time from the zoned_time
    // The local_time is a time_point in the local_t type,
    // representing the time in the specified time zone.
    std::chrono::local_time<std::chrono::system_clock::duration> local_tp = zt.get_local_time();

    // Optional: Format and print the results
    std::cout << "UTC Time: " << utc_tp << std::endl;
    std::cout << "Local Time: " << zt << std::endl; // zoned_time formats automatically to local time
    std::cout << "Local Time (explicit access): " << local_tp << std::endl;

    return 0;
}