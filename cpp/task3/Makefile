CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
all: task3
task3: main.cpp set_ops.cpp set_ops.h ../task2/set.cpp ../task2/set.h ../common/singly_list.cpp ../common/singly_list.h
	$(CXX) $(CXXFLAGS) main.cpp set_ops.cpp ../task2/set.cpp ../common/singly_list.cpp -o task3
clean:
	rm -f task3
