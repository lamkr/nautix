#include "system_metadata_provider.h"

#include <expected>
#include <sys/stat.h>
#include "../core/fs.h"
#include "../application/directory_metadata.h"

namespace nautix::infra {

    std::expected<bool, std::error_code> SystemMetadataProvider::create_directory(
        const application::DirectoryMetadata& metadata) {
        std::error_code error_code{};
        bool created = std::filesystem::create_directory(metadata.path.c_str(), error_code);
        if (error_code) {
            return std::unexpected(error_code);
        }
        return created;
    }

    [[nodiscard]] std::expected<application::DirectoryMetadata, std::error_code>
    SystemMetadataProvider::get_metadata(const char* path) const {

        struct stat st{};
        if (stat(path, &st)) {
            return std::unexpected(std::error_code(errno, std::generic_category()));
        }

        if (!S_ISDIR(st.st_mode)) {
            return std::unexpected(std::make_error_code(std::errc::not_a_directory));
        }

        return application::DirectoryMetadata(
            path,
            fs::path(path).filename(),
            compute_directory_size(path),
            st.st_uid,
            get_creation_time(st),
            get_modification_time(st),
            get_access_time(st)
        );
    }
}
