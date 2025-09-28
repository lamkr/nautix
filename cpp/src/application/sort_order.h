#pragma once

namespace nautix::application {

    enum class SortOrder {
        None,
        ByName,
        ByModificationDate,
        ByCreationDate,
        ByAccessDate,
        BySize,
        ByOwner,
    };

} // namespace nautix::application
