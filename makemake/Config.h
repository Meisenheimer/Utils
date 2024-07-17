#ifndef CONFIG_H
#define CONFIG_H

#include <cassert>
#include <vector>
#include <string>

#define MW_ALERT printf("%s %d\n", __FILE__, __LINE__);
#define MW_ERROR                                  \
    printf("Error: %s %d\n", __FILE__, __LINE__); \
    exit(0);

inline const std::string DEFAULT_OPTS = " -O2 -fopenmp -Wall -Wextra -std=c++20";
// inline const std::string DEFAULT_OPTS = " -O2 -fopenmp -Wall -Wextra -static -std=c++20";
inline const std::set<std::string> HEADER_EXTENSION = {".h", ".hh", ".hpp", ".hxx"};
inline const std::set<std::string> SOURCE_EXTENSION = {".c", ".cc", ".cpp", ".cxx"};
inline const std::set<std::string> ALL_EXTENSION = []() -> std::set<std::string>
{
    std::set<std::string> res;
    res.insert(HEADER_EXTENSION.begin(), HEADER_EXTENSION.end());
    res.insert(SOURCE_EXTENSION.begin(), SOURCE_EXTENSION.end());
    return res;
}();

inline std::string base_dir;
inline std::string main_file;
inline std::string target_file;
inline std::string opts;

inline const std::pair<std::string, std::string> splitExtension(const std::string &filename)
{
    const std::size_t index = filename.find_last_of('.');
    return std::make_pair(filename.substr(0, index), (index == std::string::npos ? "" : filename.substr(index)));
}

#endif