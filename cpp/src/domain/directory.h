#pragma once
#include <string>
#include <vector>
#include "file.h"

namespace nautix::domain {

    class Directory {
    public:
        explicit Directory(std::string path);

        [[nodiscard]] const std::string& path() const noexcept;
        void add_file(File file);
        [[nodiscard]] const std::vector<File>& files() const noexcept;

    private:
        std::string _path;
        std::vector<File> _files;
    };

} // namespace nautix::domain
