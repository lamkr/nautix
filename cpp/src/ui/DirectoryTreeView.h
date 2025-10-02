#pragma once
#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <unordered_map>

namespace nautix::ui {

    class DirectoryTreeView {
    public:
        DirectoryTreeView();

        std::vector<std::string> get_directories() const;
        void add_favorite(const std::string& dir);
        void remove_favorite(const std::string& dir);
        bool is_favorite(const std::string& dir) const;

        void navigate_to(const std::string& dir);
        std::string get_current_directory() const;

        // i18n
        void set_locale(const std::string& locale);
        std::string get_localized_label(const std::string& dir) const;

    private:
        std::vector<std::string> base_dirs;
        std::set<std::string> favorites;
        std::string current_directory;

        std::string current_locale;
        std::unordered_map<std::string, std::string> translations;
    };

} // namespace nautix::ui
