import nautix.domain;

Directory Directory::from_metadata(const nautix::application::DirectoryMetadata&& metadata) {
    return Directory(
        metadata.path,
        metadata.name.data(),
        metadata.size,
        metadata.owner_id,
        metadata.creation_time,
        metadata.modification_time,
        metadata.access_time
    );
}

STATIC std::expected<Directory, std::error_code> Directory::home(const application::IFileSystemProvider& provider) {
    const std::expected<const char*, std::error_code> path = get_home_path();
    if (path.has_value()) {
        std::expected<application::DirectoryMetadata, std::error_code> metadata =
            provider.get_metadata(*path);
        if (metadata.has_value()) {
            return from_metadata(std::move(metadata.value()));
        }
        return std::unexpected(metadata.error());
    }
    return std::unexpected(path.error());
}

STATIC std::expected<Directory, std::error_code> Directory::temp(const application::IFileSystemProvider& provider) {
    const std::expected<fs::path, std::error_code> path = fs::temp_directory_path();
    if (path.has_value()) {
        std::expected<application::DirectoryMetadata, std::error_code> metadata =
            provider.get_metadata(path->c_str());
        if (metadata.has_value()) {
            return from_metadata(std::move(metadata.value()));
        }
        return std::unexpected(metadata.error());
    }
    return std::unexpected(path.error());
}

std::expected<std::string, std::error_code> Directory::get_owner_name() const noexcept {
    return ::get_owner_name(owner_id_);
}

const std::vector<File>& Directory::files() const noexcept {
    return files_;
}

void Directory::add_file(File file) {
    files_.push_back(std::move(file));
}
