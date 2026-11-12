CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
all: task5
task5: main.cpp huffman.cpp huffman.h array.cpp array.h
	$(CXX) $(CXXFLAGS) main.cpp huffman.cpp array.cpp -o task5
clean:
	rm -f task5
