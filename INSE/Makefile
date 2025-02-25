main: Makefile main.cpp CFD.h CFD.hpp draw.py
	g++ main.cpp -o main -O3 -fopenmp -Wall -Wextra -std=c++20
	./main
	python draw.py
	python makeGIF.py

clean:
	del *.o
	del *.exe
