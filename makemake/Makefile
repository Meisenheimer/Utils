main: Makefile FileInfo.o main.o Path.o
	g++ FileInfo.o main.o Path.o -o makemake -O2 -fopenmp -Wall -Wextra -static -std=c++17

FileInfo.o: Makefile FileInfo.cpp Config.h FileInfo.h Path.h
	g++ -c FileInfo.cpp -o FileInfo.o -O2 -fopenmp -Wall -Wextra -static -std=c++17

main.o: Makefile main.cpp Config.h FileInfo.h
	g++ -c main.cpp -o main.o -O2 -fopenmp -Wall -Wextra -static -std=c++17

Path.o: Makefile Path.cpp Config.h Path.h
	g++ -c Path.cpp -o Path.o -O2 -fopenmp -Wall -Wextra -static -std=c++17

clean:
	del *.o
	del *.exe
