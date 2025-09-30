#pragma once
#include <system_error>

enum class nautix_error {
    user_not_found = 1,
};

class nautix_error_category final : public std::error_category {
public:
    const char* name() const noexcept override {
        return "nautix_error";
    }

    std::string message(int ev) const override {
        switch (static_cast<nautix_error>(ev)) {
            case nautix_error::user_not_found: return "User not found"; // TODO use language/idiom
            default: return "Unknown error";
        }
    }
};

inline const nautix_error_category& nautix_error_category_instance() {
    static nautix_error_category instance;
    return instance;
}

namespace std {
    template <>
    struct is_error_code_enum<nautix_error> : true_type {};
}

inline std::error_code make_error_code(nautix_error error) {
    return {static_cast<int>(error), nautix_error_category_instance()};
}



