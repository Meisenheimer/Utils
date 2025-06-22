ifeq ($(OS), Windows_NT)
	RM = del
else
	RM = rm
endif

makemake: Makefile main.cpp FileInfo.o Config.h FileInfo.h
	g++ main.cpp -o makemake -O2 -fopenmp -Wall -Wextra -std=c++20 FileInfo.o

FileInfo.o: Makefile FileInfo.cpp Config.h FileInfo.h
	g++ -c FileInfo.cpp -o FileInfo.o -O2 -fopenmp -Wall -Wextra -std=c++20

clean:
	$(RM) *.exe
	$(RM) *.o
