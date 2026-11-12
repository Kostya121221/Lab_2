CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
all: task7
task7: main.cpp lru_cache.cpp doubly_list.cpp lru_cache.h doubly_list.h
	$(CXX) $(CXXFLAGS) main.cpp lru_cache.cpp doubly_list.cpp -o task7
clean:
	rm -f task7
