#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <set>

#include "Config.h"

enum class FileType
{
    SOURCE,
    HEADER
};

struct FileInfo;

void writeFileInfo(const std::vector<FileInfo> &list);
const std::set<String> getDependence(const String &filename);
const std::vector<FileInfo> readFileInfo();

struct FileInfo
{
    FileType type;
    std::pair<String, String> filename;
    String fullname;
    std::set<String> dependence;

    FileInfo(const String &filename);
};

#endif