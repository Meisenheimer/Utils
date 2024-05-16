#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>

#define MW_ALERT printf("%s %d\n", __FILE__, __LINE__);

using Bool = bool;
using Integer = long int;
using Real = double;
using String = std::string;

constexpr Bool INIT_OPTS = true;

inline const std::set<String> HEADER_EXTENSION = {".h", ".hpp"};
inline const std::set<String> SOURCE_EXTENSION = {".c", ".cpp"};
inline const std::set<String> EXTENSION = {".c", ".cpp", ".h", ".hpp"};

inline String base_dir;
inline String main_file;
inline String target_file;
inline String opts;

inline const std::pair<String, String> splitExtension(const String &filename)
{
    const std::size_t index = filename.find_last_of('.');
    return std::make_pair(filename.substr(0, index), (index == String::npos ? "" : filename.substr(index)));
}

#endif