#include <sys/stat.h>

#include "directory_metadata_provider.h"
#include "../core/fs.h"

namespace nautix::infra {

    class SystemMetadataProvider final : public application::IDirectoryMetadataProvider
    {
    public:
        std::expected<application::DirectoryMetadata, std::error_code> get_metadata(const char* path) const override {

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
    };

}
