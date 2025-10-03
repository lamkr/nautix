
[[nodiscard]] std::expected<std::vector<Directory>, std::error_code>
ListDirectories::execute(const std::filesystem::path& path, SortOrder order) const {
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            std::expected<Directory, std::error_code> metadata =
                provider_.get_metadata(entry.path().c_str());
            if (!metadata.has_value()) {
                return std::unexpected(metadata.error());
            }
            metadatas.push_back(*metadata);
        }
    }

    std::ranges::sort(metadatas,
        [order](const DirectoryMetadata& a, const DirectoryMetadata& b) {
            using enum SortOrder;
            switch (order) {
                case BySize:
                    return a.size < b.size;
                case ByOwner:
                    return a.owner_id < b.owner_id;
                case ByCreationDate:
                    return a.creation_time < b.creation_time;
                case ByModificationDate:
                    return a.modification_time < b.modification_time;
                case ByAccessDate:
                    return a.access_time < b.access_time;
                case ByName:
                case None:
                default:
                    return a.name < b.name;
            }
        });

    std::vector<domain::Directory> directories;

    std::ranges::transform(metadatas,
        std::back_inserter(directories),
        [](DirectoryMetadata& metadata) {
            return domain::Directory::from_metadata(std::move(metadata));
        });

    return directories;
}

