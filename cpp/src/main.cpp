#include <iostream>
#include <string>
#include <filesystem>
#include <utility>

#include "directory.h"

using namespace nautix::domain;

// --- Exemplo de Uso Abrangente ---
int main() {
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

/*int main()
{
    nautix::application::ListDirectories useCase;

    auto subdirs = useCase.execute(nautix::domain::Directory::home().value());

    for (const auto& dir : subdirs)
    {
        std::cout << "📂 " << dir.path() << "\n";
    }
}
*/