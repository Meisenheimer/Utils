#ifndef PATH_H
#define PATH_H

#include <set>

#include "Config.h"

const std::vector<String> listdir(const String &path, const std::set<String> &extension = {});

#endif