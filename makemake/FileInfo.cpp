#include <iostream>
#include <queue>
#include <fstream>

#include "FileInfo.h"
#include "Path.h"

void writeFileInfo(const std::vector<FileInfo> &list)
{
    std::cout << std::endl;
    for (Integer i = 0; i < (Integer)list.size(); i++)
    {
        std::cout << list[i].filename.first << " " << list[i].filename.second << std::endl;
        for (auto it = list[i].dependence.begin(); it != list[i].dependence.end(); it++)
        {
            std::cout << "  --" << *it;
        }
        std::cout << std::endl;
    }
    FILE *fp = fopen("Makefile", "w");
    fprintf(fp, "%s: Makefile", main_file.c_str());
    for (Integer i = 0; i < (Integer)list.size(); i++)
    {
        if (SOURCE_EXTENSION.find(list[i].filename.second) != SOURCE_EXTENSION.end())
        {
            fprintf(fp, " %s.o", list[i].filename.first.c_str());
        }
    }
    fprintf(fp, "\n\tg++");
    for (Integer i = 0; i < (Integer)list.size(); i++)
    {
        if (SOURCE_EXTENSION.find(list[i].filename.second) != SOURCE_EXTENSION.end())
        {
            fprintf(fp, " %s.o", list[i].filename.first.c_str());
        }
    }
    fprintf(fp, " -o %s%s\n\n", target_file.c_str(), opts.c_str());

    for (Integer i = 0; i < (Integer)list.size(); i++)
    {
        if (SOURCE_EXTENSION.find(list[i].filename.second) != SOURCE_EXTENSION.end())
        {
            const String source = list[i].filename.first + list[i].filename.second;
            const String target = list[i].filename.first + ".o";
            fprintf(fp, "%s: Makefile %s", target.c_str(), list[i].fullname.c_str());
            for (auto it = list[i].dependence.begin(); it != list[i].dependence.end(); it++)
            {
                fprintf(fp, " %s", it->c_str());
            }
            fprintf(fp, "\n\tg++ -c %s -o %s%s\n\n", source.c_str(), target.c_str(), opts.c_str());
        }
    }
    fprintf(fp, "clean:\n\tdel *.o\n\tdel *.exe\n");
    fclose(fp);
    return;
}

inline const std::set<String> getDependence(const String &filename)
{
    std::set<String> list;
    std::ifstream fp(filename);
    if (fp.is_open())
    {
        while (!fp.eof())
        {
            std::size_t index = 0;
            String s;
            std::getline(fp, s);
            while ((index = s.find(' ', index)) != String::npos)
            {
                s = s.erase(index, 1);
            }
            if (s.find("#include") == 0 && s.find('\"') != String::npos)
            {
                std::size_t begin = s.find('\"') + 1;
                std::size_t end = s.find('\"', begin);
                list.insert(s.substr(begin, end - begin));
            }
        }
        fp.close();
        return list;
    }
    printf("Error At: %s %d.\n", __FILE__, __LINE__);
    exit(0);
}

inline Integer getIndex(const std::vector<FileInfo> &list, const String &filename)
{
    for (Integer i = 0; i < (Integer)list.size(); i++)
    {
        if (list[i].fullname == filename)
        {
            return i;
        }
    }
    return -1;
}

const std::vector<FileInfo> readFileInfo()
{
    std::vector<String> list = listdir("./", EXTENSION);
    std::vector<FileInfo> res;
    for (Integer i = 0; i < (Integer)list.size(); i++)
    {
        res.push_back(FileInfo(list[i]));
    }
    for (Integer i = 0; i < (Integer)list.size(); i++)
    {
        std::set<String> &dependence = res[i].dependence;
        std::queue<String> queue;
        for (auto it = dependence.begin(); it != dependence.end(); it++)
        {
            queue.push(String(*it));
            Integer index = getIndex(res, *it);
            if (index != -1)
            {
                const FileInfo &file_info = res.at(index);
                for (auto it = file_info.dependence.begin(); it != file_info.dependence.end(); it++)
                {
                    queue.push(String(*it));
                }
            }
        }
        while (!queue.empty())
        {
            String s = queue.front();
            queue.pop();
            if (dependence.find(s) == dependence.end())
            {
                dependence.insert(s);
                const FileInfo &file_info = res.at(getIndex(res, s));
                for (auto it = file_info.dependence.begin(); it != file_info.dependence.end(); it++)
                {
                    queue.push(String(*it));
                }
            }
        }
    }
    return res;
}

FileInfo::FileInfo(const String &filename)
{
    this->filename = splitExtension(filename);
    this->dependence = getDependence(filename);
    this->fullname = filename;
}