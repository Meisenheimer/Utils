#include <iostream>
#include <unistd.h>

#include "FileInfo.h"

void parseArgs(const int argc, const char *argv[]);

int main(const int argc, const char *argv[])
{
    parseArgs(argc, argv);
    writeFileInfo(readFileInfo());
    return 0;
}

inline void parseArgs(const int argc, const char *argv[])
{
    if (argc < 4)
    {
        printf("Usage: makemake base_dir main_file target_file opts\n");
        exit(0);
    }
    base_dir = argv[1];
    main_file = argv[2];
    target_file = argv[3];
    opts = INIT_OPTS ? String(" -O2 -fopenmp -Wall -Wextra -static -std=c++17") : String("");
    for (Integer i = 4; i < argc; i++)
    {
        opts += " " + String(argv[i]);
    }
    std::cout << "--base_dir = " << base_dir << std::endl;
    std::cout << "--main_file = " << main_file << std::endl;
    std::cout << "--target_file = " << target_file << std::endl;
    std::cout << "--opts = " << opts << std::endl;
    chdir(base_dir.c_str());
    return;
}