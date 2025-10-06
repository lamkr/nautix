#pragma once
#include <system_error>

enum class nautix_error {
    user_not_found = 1,
    directory_not_found = 2,
    move_cleanup_failed = 3,
};

class nautix_error_category final : public std::error_category {
    const std::error_code cause_;
public:

    explicit nautix_error_category(const std::error_code cause)
        : std::error_category()
        , cause_(cause)
        {}


    [[nodiscard]] bool containsCause() const noexcept {
        return cause_ != std::error_code{};
    }

    [[nodiscard]] std::error_code cause() const noexcept {
        return cause_;
    }

    [[nodiscard]] const char* name() const noexcept override {
        return "nautix_error";
    }

    [[nodiscard]] std::string message(int ev) const override {
        switch (static_cast<nautix_error>(ev)) {
            case nautix_error::user_not_found: return "User not found"; // TODO use language/idiom
            case nautix_error::directory_not_found: return "Directory not found"; // TODO use language/idiom
            case nautix_error::move_cleanup_failed: return "Move cleanup failed"; // TODO use language/idiom
            default: return "Unknown error";
        }
    }
};

inline const nautix_error_category& nautix_error_category_instance(const std::error_code& cause) {
    static nautix_error_category instance(cause);
    return instance;
}

inline std::error_code make_error_code(nautix_error error, const std::error_code& cause) {
    return {static_cast<int>(error), nautix_error_category_instance(cause)};
}

inline const nautix_error_category& nautix_error_category_instance() {
    return nautix_error_category_instance(std::error_code{});
}

inline std::error_code make_error_code(nautix_error error) {
    return {static_cast<int>(error), nautix_error_category_instance()};
}

namespace std {
    template <>
    struct is_error_code_enum<nautix_error> : true_type {};
}
