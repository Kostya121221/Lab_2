CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
TARGET = task1

all:
	$(CXX) $(CXXFLAGS) main.cpp expression.cpp stack.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
