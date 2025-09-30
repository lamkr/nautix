#pragma once

namespace nautix::application {

    enum class SortOrder {
        None,
        ByName,
        BySize,
        ByOwner,
        ByCreationDate,
        ByModificationDate,
        ByAccessDate,
    };

} // namespace nautix::application
