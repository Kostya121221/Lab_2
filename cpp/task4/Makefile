CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
all: task4
task4: main.cpp wildcard.cpp wildcard.h array.cpp array.h
	$(CXX) $(CXXFLAGS) main.cpp wildcard.cpp array.cpp -o task4
clean:
	rm -f task4
