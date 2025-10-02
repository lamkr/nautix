export module nautix.domain:directory;

#include <chrono>
#include <filesystem>
import :local_time;
import :owner;

export class Directory
{
    const std::filesystem::path path_{};
    const std::string name_{};
    const uintmax_t size_{0};
    const Owner owner_{};
    const LocalTime creation_time_{};
    const LocalTime modification_time_{};
    const LocalTime access_time_{};

public:
    [[nodiscard]] const std::filesystem::path& path() const noexcept { return path_; }
    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    [[nodiscard]] uintmax_t size() const noexcept { return size_; }
    [[nodiscard]] const Owner& owner() const noexcept { return owner_; }
    [[nodiscard]] const LocalTime& creation_time() const noexcept { return creation_time_; }
    [[nodiscard]] const LocalTime& modification_time() const noexcept { return modification_time_; }
    [[nodiscard]] const LocalTime& access_time() const noexcept { return access_time_; }

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
        const Owner&& owner_id,
        const LocalTime&& creation_time,
        const LocalTime&& modification_time,
        const LocalTime&& access_time
        )
        : path_(path_param)//(std::forward<PathType>(path_param))
        , name_(name)//(std::forward<std::string>(name))
        , size_(size)
        , owner_(owner_id)
        , creation_time_(creation_time)
        , modification_time_(modification_time)
        , access_time_(access_time)
    {}
};
