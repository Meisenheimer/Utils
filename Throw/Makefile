main: Makefile Curve.o main.cpp
	g++ Curve.o main.cpp -o main -O2 -fopenmp -Wall -Wextra -static -std=c++20 -lgraphics -lgdiplus -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm

Curve.o: Makefile Curve.cpp Config.h Curve.h
	g++ -c Curve.cpp -o Curve.o -O2 -fopenmp -Wall -Wextra -static -std=c++20 -lgraphics -lgdiplus -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm

clean:
	del *.o
	del *.exe
