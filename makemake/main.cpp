#include <iostream>
#include <unistd.h>

#include "FileInfo.h"

void parseArgs(const int argc, const char *argv[]);

int main(const int argc, const char *argv[])
{
    parseArgs(argc, argv);
    loadInclude();
    loadModule();
    saveModule();
    return 0;
}

inline void parseArgs(const int argc, const char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: makemake main_file [-d base_dir] [-o target_file] [opts]\n");
        exit(0);
    }
    std::pair<std::string, std::string> tmp = splitExtension(argv[1]);
    main_file = tmp.first;
    base_dir = "./";
    target_file = "";
    opts = "";
    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
        {
            assert(base_dir == "./");
            assert((i + 1) < argc);
            base_dir = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            assert(target_file == "");
            assert((i + 1) < argc);
            target_file = argv[i + 1];
            i++;
        }
        else
        {
            opts.append(" ");
            opts.append(argv[i]);
        }
    }
    chdir(base_dir.c_str());
    if (target_file == "")
    {
        target_file = main_file;
    }
    if (opts == "")
    {
        opts = DEFAULT_OPTS;
    }
    std::cout << "--base_dir = " << base_dir << std::endl;
    std::cout << "--main_file = " << main_file << std::endl;
    std::cout << "--target_file = " << target_file << std::endl;
    std::cout << "--opts = " << opts << std::endl;
    std::cout << std::endl;
    return;
}