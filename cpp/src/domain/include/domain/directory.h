#pragma once
#include <chrono>
#include <filesystem>

#include "local_time.h"
#include "owner.h"

namespace nautix::domain {

    class Directory {
        std::filesystem::path path_;
        std::string name_;
        uintmax_t size_;
        Owner owner_;
        LocalTime creation_time_;
        LocalTime modification_time_;
        LocalTime access_time_;

    public:
        // Construtor padrão para facilitar a criação de mocks e testes
        Directory() = default;

        // C++20 Concept: Garante que este construtor só seja habilitado para
        // tipos que podem construir um std::filesystem::path e que não seja
        // uma tentativa de usar o construtor de cópia/movimento da própria classe.
        template <typename PathType>
            requires(
                !std::is_same_v<std::remove_cvref_t<PathType>, Directory> &&
                std::is_constructible_v<std::filesystem::path, PathType>
            )
        explicit Directory(
            PathType&& path,
            std::string name,
            const uintmax_t size,
            Owner owner,
            const LocalTime creation_time,
            const LocalTime modification_time,
            const LocalTime access_time)
            :
            path_(std::forward<PathType>(path)),
            name_(std::move(name)),
            size_(size),
            owner_(std::move(owner)),
            creation_time_(creation_time),
            modification_time_(modification_time),
            access_time_(access_time) {}

        [[nodiscard]] const std::filesystem::path& path() const noexcept { return path_; }
        [[nodiscard]] const std::string& name() const noexcept { return name_; }
        [[nodiscard]] uintmax_t size() const noexcept { return size_; }
        [[nodiscard]] const Owner& owner() const noexcept { return owner_; }
        [[nodiscard]] const LocalTime& creation_time() const noexcept { return creation_time_; }
        [[nodiscard]] const LocalTime& modification_time() const noexcept { return modification_time_; }
        [[nodiscard]] const LocalTime& access_time() const noexcept { return access_time_; }
    };
}
