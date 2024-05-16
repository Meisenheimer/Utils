#include <dirent.h>

#include "Path.h"

const std::vector<String> listdir(const String &path, const std::set<String> &extension)
{
    // https://blog.csdn.net/qq289665044/article/details/48623325
    std::vector<String> list;
    DIR *dp;
    struct dirent *entry;
    if ((dp = opendir(path.c_str())) != NULL)
    {
        while ((entry = readdir(dp)) != NULL)
        {
            Bool flag = true;
            if (extension.size() > 0)
            {
                const String ext = splitExtension(entry->d_name).second;
                flag = false;
                if (extension.find(ext) != extension.end())
                {
                    flag = true;
                }
            }
            if (flag)
            {
                list.push_back(String(entry->d_name));
            }
        }
        closedir(dp);
    }
    return list;
}