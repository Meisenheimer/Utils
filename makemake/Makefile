main: Makefile main.cpp FileInfo.o Config.h FileInfo.h
	g++ main.cpp -o main -O2 -fopenmp -Wall -Wextra -static -std=c++20 FileInfo.o

FileInfo.o: Makefile FileInfo.cpp Config.h FileInfo.h
	g++ -c FileInfo.cpp -o FileInfo.o -O2 -fopenmp -Wall -Wextra -static -std=c++20

clean:
	del *.o
	del *.exe
