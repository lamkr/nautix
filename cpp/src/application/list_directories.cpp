#include <filesystem>
#include <sys/stat.h>
#include <system_error>
#include "list_directories.h"

namespace nautix::application
{
    using namespace nautix::domain;
    namespace fs = std::filesystem;

    std::vector<Directory> ListDirectories::execute(const Directory& root, SortOrder order) const
    {
        std::vector<fs::directory_entry> dirs;
        std::vector<Directory> result;
        try
        {
            if (!fs::exists(root.path()) || !fs::is_directory(root.path()))
            {
                return result;
            }

            for (const fs::directory_entry& entry : fs::directory_iterator(root.path()))
            {
                if (entry.is_directory())
                {
                    dirs.emplace_back(entry);
                }
            }

            switch (order)
            {
                using enum SortOrder;

            case ByName:
                std::sort(dirs.begin(), dirs.end(),
                          [](const fs::directory_entry& a, const fs::directory_entry& b)
                          {
                              return a.path() < b.path();
                          });
                break;

            case ByCreationDate:
                std::sort(dirs.begin(), dirs.end(),
                          [](const fs::directory_entry& a, const fs::directory_entry& b)
                          {
                              struct stat statA{}, statB{};
                              if (stat(a.path().c_str(), &statA) != 0) return false;
                              if (stat(b.path().c_str(), &statB) != 0) return true;
#if defined(__APPLE__)
                              return statA.st_birthtime < statB.st_birthtime;
#else
                              // On Linux, birth time is not standard; using st_ctim as a fallback
                              return statA.st_ctim.tv_nsec < statB.st_ctim.tv_nsec;
#endif
                          });
                break;

            case ByModificationDate:
                std::sort(dirs.begin(), dirs.end(),
                          [](const fs::directory_entry& a, const fs::directory_entry& b)
                          {
                              const auto timeA = fs::last_write_time(a);
                              const auto timeB = fs::last_write_time(b);
                              return timeA < timeB;
                          });
                break;

            case ByAccessDate:
                std::sort(dirs.begin(), dirs.end(),
                          [](const fs::directory_entry& a, const fs::directory_entry& b)
                          {
                              struct stat statA{}, statB{};
                              if (stat(a.path().c_str(), &statA) != 0) return false;
                              if (stat(b.path().c_str(), &statB) != 0) return true;
                              return statA.st_atime < statB.st_atime;
                          });
                break;

            case BySize:
                std::sort(dirs.begin(), dirs.end(),
                          [](const fs::directory_entry& a, const fs::directory_entry& b)
                          {
                              std::uintmax_t sizeA = 0, sizeB = 0;
                              std::error_code errorA, errorB;
                              sizeA = fs::file_size(a, errorA);
                              sizeB = fs::file_size(b, errorB);
                              return !(errorA && errorB) ? sizeA < sizeB : false;
                          });
                break;

            case ByOwner:
                std::sort(dirs.begin(), dirs.end(),
                          [](const fs::directory_entry& a, const fs::directory_entry& b)
                          {
                              struct stat statA{}, statB{};
                              if (stat(a.path().c_str(), &statA) != 0) return false;
                              if (stat(b.path().c_str(), &statB) != 0) return true;
                              return statA.st_uid < statB.st_uid;
                          });
                break;

            case None:
            default:
                break;
            }
        }
        catch (const fs::filesystem_error&)
        {
            // Em TDD: ainda não tratamos erros específicos
        }

        for (const fs::directory_entry& entry : dirs)
        {
            result.emplace_back(Directory(entry));
        }
        return result;
    }
} // namespace nautix::application
