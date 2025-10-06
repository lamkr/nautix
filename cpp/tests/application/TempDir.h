#pragma once
#include <filesystem>
#include <unistd.h>

// Helper to create temporary test environment.
struct TempDir {
    std::filesystem::path path;

    TempDir() {
        path = std::filesystem::temp_directory_path() /
            std::filesystem::path("nautix_test_" + std::to_string(::getpid()));
        std::filesystem::create_directory(path);
    }

    ~TempDir() {
        std::filesystem::remove_all(path);
    }
};
