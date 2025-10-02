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

        std::vector<std::string> getDirectories() const;
        void addFavorite(const std::string& dir);
        void removeFavorite(const std::string& dir);
        bool isFavorite(const std::string& dir) const;

        void navigateTo(const std::string& dir);
        std::string getCurrentDirectory() const;

        // i18n
        void setLocale(const std::string& locale);
        std::string getLocalizedLabel(const std::string& dir) const;

    private:
        std::vector<std::string> baseDirs;
        std::set<std::string> favorites;
        std::string currentDir;

        std::string currentLocale;
        std::unordered_map<std::string, std::string> translations;
    };

} // namespace nautix::ui
