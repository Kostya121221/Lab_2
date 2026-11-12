CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
all: task6
task6: main.cpp hash_roman.cpp hash_roman.h array.cpp array.h
	$(CXX) $(CXXFLAGS) main.cpp hash_roman.cpp array.cpp -o task6
clean:
	rm -f task6
