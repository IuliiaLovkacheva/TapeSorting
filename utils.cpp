#include <filesystem>
#include "utils.h"

void createDirectoryIfMissing(const std::string& directoryName) {
    if (!std::filesystem::is_directory(directoryName) ||
        !std::filesystem::exists(directoryName)) {
        std::filesystem::create_directory(directoryName);
    }
}
