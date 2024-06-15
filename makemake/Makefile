main: Makefile FileInfo.o main.cpp Path.o
	g++ FileInfo.o main.cpp Path.o -o makemake -O2 -fopenmp -Wall -Wextra -static -std=c++20

FileInfo.o: Makefile FileInfo.cpp Config.h FileInfo.h Path.h
	g++ -c FileInfo.cpp -o FileInfo.o -O2 -fopenmp -Wall -Wextra -static -std=c++20

Path.o: Makefile Path.cpp Config.h Path.h
	g++ -c Path.cpp -o Path.o -O2 -fopenmp -Wall -Wextra -static -std=c++20

clean:
	del *.o
	del *.exe
