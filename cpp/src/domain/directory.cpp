#include "directory.h"
#include "file.h"

namespace nautix::domain {

    Directory::Directory(std::string path)
        : _path(std::move(path)) {}

    const std::string& Directory::path() const noexcept {
        return _path;
    }

    void Directory::add_file(File file) {
        _files.push_back(std::move(file));
    }

    const std::vector<File>& Directory::files() const noexcept {
        return _files;
    }

} // namespace nautix::domain
