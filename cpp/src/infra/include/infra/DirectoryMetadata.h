#pragma once

#include <filesystem>

#include "domain/local_time.h"
#include "domain/owner.h"

namespace nautix::infra {
    struct DirectoryMetadata {
        std::filesystem::path path;
        std::string name;
        std::uintmax_t size{0};
        domain::Owner owner{};
        domain::LocalTime creation_time;
        domain::LocalTime modification_time;
        domain::LocalTime access_time;
    };
}
