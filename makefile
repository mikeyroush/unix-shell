# makefile

all: test

Commands.o: Commands.cpp
	g++ -c -g Commands.cpp

Parser.o: Parser.cpp
	g++ -c -g Parser.cpp

Main.o: Main.cpp
	g++ -c -g Main.cpp

test: Main.o Parser.o Commands.o
	g++ -o test Main.o Parser.o Commands.o

clean:
	rm *.o