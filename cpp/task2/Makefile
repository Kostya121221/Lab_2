CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
all: task2
task2: main.cpp set.cpp set.h ../common/singly_list.cpp ../common/singly_list.h
	$(CXX) $(CXXFLAGS) main.cpp set.cpp ../common/singly_list.cpp -o task2
clean:
	rm -f task2
