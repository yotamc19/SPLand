# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

# CXXFLAGS: Use C++17, enable warnings (-Wall, -Wextra), and include debug info (-g).
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Default target to build, clean, and run everything
all: clean compile run

# Compile target to build the C++ executable
compile: Action.o Auxiliary.o Facility.o Plan.o SelectionPolicy.o Settlement.o Simulation.o main.o
	g++ -o bin/run bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o bin/main.o

Action.o:
	g++ $(CXXFLAGS) -I include/ -o bin/Action.o -c src/Action.cpp

Auxiliary.o:
	g++ $(CXXFLAGS) -I include/ -o bin/Auxiliary.o -c src/Auxiliary.cpp

Facility.o:
	g++ $(CXXFLAGS) -I include/ -o bin/Facility.o -c src/Facility.cpp

Plan.o:
	g++ $(CXXFLAGS) -I include/ -o bin/Plan.o -c src/Plan.cpp

SelectionPolicy.o:
	g++ $(CXXFLAGS) -I include/ -o bin/SelectionPolicy.o -c src/SelectionPolicy.cpp

Settlement.o:
	g++ $(CXXFLAGS) -I include/ -o bin/Settlement.o -c src/Settlement.cpp

Simulation.o:
	g++ $(CXXFLAGS) -I include/ -o bin/Simulation.o -c src/Simulation.cpp
	
main.o:
	g++ $(CXXFLAGS) -I include/ -o bin/main.o -c src/main.cpp

# Run target to execute the program
run:
	./bin/run config_file.txt

# Clean target to remove the executable
clean: 
	rm -f bin/run bin/*.o