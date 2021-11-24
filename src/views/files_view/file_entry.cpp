#include "file_entry.hpp"

FileEntry::FileEntry(const std::filesystem::path& path)
    : path_(path)
{
}

std::string FileEntry::filename() const
{
    return path_.filename().string();
}
