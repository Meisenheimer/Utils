main: Makefile Curve.o main.o
	g++ Curve.o main.o -o main -O2 -fopenmp -Wall -Wextra -static -std=c++17 -lEasyX -lgdi32 -lole32

Curve.o: Makefile Curve.cpp Config.h Curve.h
	g++ -c Curve.cpp -o Curve.o -O2 -fopenmp -Wall -Wextra -static -std=c++17 -lEasyX -lgdi32 -lole32

main.o: Makefile main.cpp Config.h Curve.h
	g++ -c main.cpp -o main.o -O2 -fopenmp -Wall -Wextra -static -std=c++17 -lEasyX -lgdi32 -lole32

clean:
	del *.o
	del *.exe
