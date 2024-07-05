#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <fstream>
#include <set>
#include <map>

#include "Config.h"

struct Module;

extern std::set<std::string> source;
extern std::map<std::string, std::set<std::string>> include;

std::set<std::string> findFile(const std::string &name, const std::set<std::string> &extension,
                               const std::string &path = base_dir);
std::set<std::string> listdir(const std::set<std::string> &extension, const std::string &path = base_dir);
std::set<std::string> readInclude(const std::string &filename);

void loadInclude();
void loadInclude(const std::string &filename);
void loadModule();
void saveModule();

#endif