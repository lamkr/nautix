#pragma once
#include <filesystem>
#include <vector>
#include "file.h"
#include <type_traits>

namespace nautix::domain {

    class Directory {
    public:
        [[nodiscard]] static const std::optional<Directory>& home();

        /**
         * @brief Construtor universal que aceita qualquer tipo que possa construir
         * um std::filesystem::path de forma performática.
         * * @tparam PathType O tipo do argumento do caminho (deduzido automaticamente).
         * @param path_param O parâmetro do caminho, passado como uma referência de
         * encaminhamento (forwarding reference).
         */
        template <typename PathType>
            // C++20 Concept: Garante que este construtor só seja habilitado para
            // tipos que podem construir um std::filesystem::path e que não seja
            // uma tentativa de usar o construtor de cópia/movimento da própria classe.
            requires(
                !std::is_same_v<std::remove_cvref_t<PathType>, Directory> &&
                 std::is_constructible_v<std::filesystem::path, PathType>
            )
        explicit Directory(PathType&& path_param)
            : _path(std::forward<PathType>(path_param)) {}


        [[nodiscard]] const std::filesystem::path& path() const noexcept;
        [[nodiscard]] const std::vector<File>& files() const noexcept;
        void add_file(File file);

    private:
        std::filesystem::path _path;
        std::vector<File> _files;
    };

}
