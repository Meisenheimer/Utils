#include <iostream>
#include <fstream>
#include <dirent.h>

#include "FileInfo.h"

std::set<std::string> source;
std::map<std::string, std::set<std::string>> include;

std::set<std::string> findFile(const std::string &name, const std::set<std::string> &extension, const std::string &path)
{
    std::set<std::string> list;
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(path.c_str())) != NULL)
    {
        while ((entry = readdir(dp)) != NULL)
        {
            const std::string filename = entry->d_name;
            bool flag = true;
            if (extension.size() > 0)
            {
                const std::pair<std::string, std::string> split = splitExtension(filename);
                flag = false;
                if (name == split.first && extension.contains(split.second))
                {
                    flag = true;
                }
            }
            if (flag)
            {
                list.insert(std::string(filename));
            }
        }
        closedir(dp);
    }
    return list;
}

std::set<std::string> listdir(const std::set<std::string> &extension, const std::string &path)
{
    std::set<std::string> list;
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(path.c_str())) != NULL)
    {
        while ((entry = readdir(dp)) != NULL)
        {
            bool flag = true;
            if (extension.size() > 0)
            {
                const std::string ext = splitExtension(entry->d_name).second;
                flag = false;
                if (extension.find(ext) != extension.end())
                {
                    flag = true;
                }
            }
            if (flag)
            {
                list.insert(std::string(entry->d_name));
            }
        }
        closedir(dp);
    }
    return list;
}

std::set<std::string> readInclude(const std::string &filename)
{
    std::set<std::string> list;
    std::ifstream fp(filename);
    assert(fp.is_open());
    while (!fp.eof())
    {
        std::string s;
        std::string tmp;
        std::getline(fp, tmp);
        for (size_t i = 0; i < tmp.length(); i++)
        {
            if (tmp.at(i) != ' ')
            {
                s += tmp.at(i);
            }
        }
        if (s.find("#include") == 0 && s.find('\"') != std::string::npos)
        {
            std::size_t begin = s.find('\"') + 1;
            std::size_t end = s.find('\"', begin);
            list.insert(s.substr(begin, end - begin));
        }
    }
    fp.close();
    return list;
}

void loadInclude()
{
    include.clear();
    auto list = listdir(ALL_EXTENSION);
    for (auto it = list.begin(); it != list.end(); it++)
    {
        loadInclude(*it);
    }
    return;
}

void loadInclude(const std::string &filename)
{
    std::set<std::string> list = readInclude(filename);
    std::vector<std::string> queue;
    queue.assign(list.begin(), list.end());
    list.clear();
    include[filename] = std::set<std::string>();
    while (!queue.empty())
    {
        const std::string name = queue.back();
        queue.pop_back();
        if (!list.contains(name))
        {
            list.insert(name);
            if (!include.contains(name))
            {
                loadInclude(name);
            }
            queue.insert(queue.end(), include[name].begin(), include[name].end());
        }
    }
    include[filename] = list;
    return;
}

void loadModule()
{
    std::set<std::string> list = findFile(main_file, SOURCE_EXTENSION);
    assert(list.size() == 1);
    const std::string filename = *list.begin();
    for (auto it = include[filename].begin(); it != include[filename].end(); it++)
    {
        std::set<std::string> tmp = findFile(splitExtension(*it).first, SOURCE_EXTENSION);
        assert(tmp.size() <= 1);
        if (!tmp.empty())
        {
            source.merge(tmp);
        }
    }
    return;
}

void saveModule()
{
    const std::string fullname = *findFile(main_file, SOURCE_EXTENSION).begin();
    std::cout << "Target: " << fullname << " -> " << target_file << std::endl;
    if (!include[main_file].empty())
    {
        std::cout << "Related: ";
        for (auto jt = include[main_file].begin(); jt != include[main_file].end(); jt++)
        {
            std::cout << *jt << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (auto it = source.begin(); it != source.end(); it++)
    {
        std::cout << "Module: " << *it << std::endl;
        if (!include[*it].empty())
        {
            std::cout << "Related: ";
            for (auto jt = include[*it].begin(); jt != include[*it].end(); jt++)
            {
                std::cout << *jt << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    std::ofstream fp("Makefile", std::ios::out);
    assert(fp.is_open());
    fp << target_file << ": Makefile " << fullname;
    for (auto it = source.begin(); it != source.end(); it++)
    {
        fp << " " << splitExtension(*it).first << ".o";
    }
    for (auto it = include[fullname].begin(); it != include[fullname].end(); it++)
    {
        fp << " " << *it;
    }
    fp << std::endl;
    fp << "\tg++ " << fullname << " -o " << target_file << opts;
    for (auto it = source.begin(); it != source.end(); it++)
    {
        fp << " " << splitExtension(*it).first << ".o";
    }
    fp << std::endl;
    fp << std::endl;

    for (auto it = source.begin(); it != source.end(); it++)
    {
        fp << splitExtension(*it).first << ".o: Makefile " << *it;
        for (auto jt = include[*it].begin(); jt != include[*it].end(); jt++)
        {
            fp << " " << *jt;
        }
        fp << std::endl;
        fp << "\tg++ -c " << *it << " -o " << splitExtension(*it).first << ".o" << opts;
        fp << std::endl;
        fp << std::endl;
    }

#ifdef __linux__
    fp << "clean:\n\trm *.o\n\trm *.exe\n";
#elif defined(_WIN32)
    fp << "clean:\n\tdel *.o\n\tdel *.exe\n";
#endif
    fp.close();

    return;
}
