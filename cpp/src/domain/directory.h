#pragma once

#include <chrono>
#include <filesystem>
#include "file.h"
#include "../application/directory_metadata_provider.h"

namespace nautix::domain {

    namespace chrono = std::chrono;
    namespace fs = std::filesystem;
    namespace app = nautix::application;

    class Directory
    {
        const fs::path path_;
        const std::string name_;
        const uintmax_t size_;
        const uid_t owner_id_;
        const chrono::local_time<chrono::system_clock::duration> creation_time_;
        const chrono::local_time<chrono::system_clock::duration> modification_time_;
        const chrono::local_time<chrono::system_clock::duration> access_time_;

        std::vector<File> files_;

    public:
        [[nodiscard]] static std::expected<Directory, std::error_code> home(const app::IDirectoryMetadataProvider& provider);
        [[nodiscard]] static std::expected<Directory, std::error_code> temp(const app::IDirectoryMetadataProvider& provider);
        [[nodiscard]] static Directory from_metadata(const application::DirectoryMetadata&& metadata);

        [[nodiscard]] const fs::path& path() const noexcept { return path_; }
        [[nodiscard]] const std::string& name() const noexcept { return name_; }
        [[nodiscard]] uintmax_t size() const noexcept { return size_; }
        [[nodiscard]] uid_t owner_id() const noexcept { return owner_id_; }
        // Em Linux, creation time (birthtime) não é portable; st_birthtime existe em alguns FS/OS,
        // mas em Linux frequentemente não está disponível. O código atual usa fallback st_ctime.
        [[nodiscard]] const chrono::local_time<chrono::system_clock::duration>& creation_time() const noexcept { return creation_time_; }
        [[nodiscard]] const chrono::local_time<chrono::system_clock::duration>& modification_time() const noexcept { return modification_time_; }
        [[nodiscard]] const chrono::local_time<chrono::system_clock::duration>& access_time() const noexcept { return access_time_; }
        [[nodiscard]] std::expected<std::string, std::error_code> get_owner_name() const noexcept;

        [[nodiscard]] const std::vector<File>& files() const noexcept;
        void add_file(File file);

        template <typename PathType>
            // C++20 Concept: Garante que este construtor só seja habilitado para
            // tipos que podem construir um std::filesystem::path e que não seja
            // uma tentativa de usar o construtor de cópia/movimento da própria classe.
            requires(
                !std::is_same_v<std::remove_cvref_t<PathType>, Directory> &&
                 std::is_constructible_v<std::filesystem::path, PathType>
            )
        explicit Directory(
            PathType&& path_param,
            std::string&& name,
            const uintmax_t size,
            const uid_t owner_id,
            chrono::local_time<chrono::system_clock::duration> creation_time,
            chrono::local_time<chrono::system_clock::duration> modification_time,
            chrono::local_time<chrono::system_clock::duration> access_time
            )
            : path_(std::forward<PathType>(path_param))
            , name_(std::forward<std::string>(name))
            , size_(size)
            , owner_id_(owner_id)
            , creation_time_(std::forward<chrono::local_time<chrono::system_clock::duration>>(creation_time))
            , modification_time_(std::forward<chrono::local_time<chrono::system_clock::duration>>(modification_time))
            , access_time_(std::forward<chrono::local_time<chrono::system_clock::duration>>(access_time))
        {}
    };
}
