#include "file_entry.hpp"

namespace video_trimmer::views::files_view {

FileEntry::FileEntry(const std::filesystem::path& path)
    : path_(path)
{
}

std::string FileEntry::filename() const
{
    return path_.filename().string();
}

}  // namespace video_trimmer::views::files_view
