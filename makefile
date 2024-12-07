FLAGS = -g -Wall -Weffc++ -std=c++11 -Iinclude

all: clean compile link run

clean: 
	rm -f bin/*

compile:
	g++ $(FLAGS) -c -o bin/main.o src/main.cpp
	g++ $(FLAGS) -c -o bin/Action.o src/Action.cpp
	g++ $(FLAGS) -c -o bin/Auxiliary.o src/Auxiliary.cpp
	g++ $(FLAGS) -c -o bin/Facility.o src/Facility.cpp
	g++ $(FLAGS) -c -o bin/Plan.o src/Plan.cpp
	g++ $(FLAGS) -c -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ $(FLAGS) -c -o bin/Settlement.o src/Settlement.cpp
	g++ $(FLAGS) -c -o bin/Simulation.o src/Simulation.cpp

link:
	g++ -o bin/simulation bin/main.o bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o

run:
	./bin/simulation config_file.txt

c: clean compile link 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/simulation config_file.txt