# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

# CXXFLAGS: Use C++17, enable warnings (-Wall, -Wextra), and include debug info (-g).
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Default target to build, clean, and run everything
all: clean compile run

# Compile target to build the C++ executable
compile: Settlement.o Facility.o main.o
	g++ -o bin/run bin/Settlement.o bin/Facility.o bin/main.o
	
main.o:
	g++ $(CXXFLAGS) -I include/ -o bin/main.o -c src/main.cpp

Settlement.o:
	g++ $(CXXFLAGS) -I include/ -o bin/Settlement.o -c src/Settlement.cpp

Facility.o:
	g++ $(CXXFLAGS) -I include/ -o bin/Facility.o -c src/Facility.cpp

# Run target to execute the program
run:
	./bin/run config_file.txt

# Clean target to remove the executable
clean: 
	rm -f bin/run bin/*.o