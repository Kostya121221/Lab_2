#include "lru_cache.h"

#include <iostream>
#include <string>

int main() {
    int capacity, queryCount;
    if (!(std::cin >> capacity >> queryCount)) return 0;

    LRUCache cache{};
    initLRUCache(cache, capacity);

    for (int i = 0; i < queryCount; ++i) {
        std::string operation;
        int key;
        if (!(std::cin >> operation >> key)) break;

        if (operation == "GET") {
            std::cout << lruGet(cache, key) << ' ';
        } else if (operation == "SET") {
            int value;
            if (!(std::cin >> value)) break;
            lruSet(cache, key, value);
        }
    }

    std::cout << '\n';
    clearLRUCache(cache);
    return 0;
}
