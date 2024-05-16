main.cpp: Makefile Chess.o Config.o main.o Strategy.o
	g++ Chess.o Config.o main.o Strategy.o -o main -O2 -fopenmp -Wall -Wextra -static -std=c++17 -O2 -fopenmp -Wall -Wextra -static -std=c++17 -lEasyX -lgdi32 -lole32

Chess.o: Makefile Chess.cpp Chess.h Config.h
	g++ -c Chess.cpp -o Chess.o -O2 -fopenmp -Wall -Wextra -static -std=c++17 -O2 -fopenmp -Wall -Wextra -static -std=c++17 -lEasyX -lgdi32 -lole32

Config.o: Makefile Config.cpp Config.h
	g++ -c Config.cpp -o Config.o -O2 -fopenmp -Wall -Wextra -static -std=c++17 -O2 -fopenmp -Wall -Wextra -static -std=c++17 -lEasyX -lgdi32 -lole32

main.o: Makefile main.cpp Chess.h Config.h Strategy.h
	g++ -c main.cpp -o main.o -O2 -fopenmp -Wall -Wextra -static -std=c++17 -O2 -fopenmp -Wall -Wextra -static -std=c++17 -lEasyX -lgdi32 -lole32

Strategy.o: Makefile Strategy.cpp Chess.h Config.h Strategy.h
	g++ -c Strategy.cpp -o Strategy.o -O2 -fopenmp -Wall -Wextra -static -std=c++17 -O2 -fopenmp -Wall -Wextra -static -std=c++17 -lEasyX -lgdi32 -lole32

clean:
	del *.o
	del *.exe
