main: Makefile Chess.o Config.o main.cpp Strategy.o
	g++ Chess.o Config.o main.cpp Strategy.o -o main -O2 -fopenmp -Wall -Wextra -static -std=c++20 -lgraphics -lgdiplus -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm

Chess.o: Makefile Chess.cpp Chess.h Config.h
	g++ -c Chess.cpp -o Chess.o -O2 -fopenmp -Wall -Wextra -static -std=c++20 -lgraphics -lgdiplus -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm

Config.o: Makefile Config.cpp Config.h
	g++ -c Config.cpp -o Config.o -O2 -fopenmp -Wall -Wextra -static -std=c++20 -lgraphics -lgdiplus -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm

Strategy.o: Makefile Strategy.cpp Chess.h Config.h Strategy.h
	g++ -c Strategy.cpp -o Strategy.o -O2 -fopenmp -Wall -Wextra -static -std=c++20 -lgraphics -lgdiplus -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm

clean:
	del *.o
	del *.exe
