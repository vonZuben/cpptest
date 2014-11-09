all: clean main.o
	g++ -std=c++11 main.o -o program -O0 -g

main.o: main.s
	g++ -std=c++11 -c -o main.o main.s -O0 -g

main.s:
	g++ -std=c++11 -S main.cpp -O0 -g

clean:
	rm -rf main.s main.o

run:
	./program
