FLAGS=-O0 -g

all: clean main.o
	g++ -std=c++11 main.o -o program ${FLAGS}

main.o: main.s
	g++ -std=c++11 -c -o main.o main.s ${FLAGS}

main.s:
	g++ -std=c++11 -S main.cpp ${FLAGS}

clean:
	rm -rf main.s main.o

run:
	./program
