#include "lru_cache.h"

#include <string>

void initLRUCache(LRUCache& cache, int capacity) {
    clearLRUCache(cache);
    cache.capacity = capacity > 0 ? capacity : 0;
}

void clearLRUCache(LRUCache& cache) {
    clearDoublyList(cache.order);
    cache.nodes.clear();
    cache.values.clear();
    cache.capacity = 0;
}

int lruGet(LRUCache& cache, int key) {
    auto found = cache.nodes.find(key);
    if (found == cache.nodes.end()) return -1;

    doublyMoveToFront(cache.order, found->second);
    auto value = cache.values.find(key);
    return value == cache.values.end() ? -1 : value->second;
}

void lruSet(LRUCache& cache, int key, int value) {
    if (cache.capacity <= 0) return;

    auto found = cache.nodes.find(key);
    if (found != cache.nodes.end()) {
        cache.values[key] = value;
        doublyMoveToFront(cache.order, found->second);
        return;
    }

    if (static_cast<int>(cache.nodes.size()) >= cache.capacity) {
        if (cache.order.tail != nullptr) {
            const int oldKey = std::stoi(cache.order.tail->value);
            doublyEraseBack(cache.order);
            cache.nodes.erase(oldKey);
            cache.values.erase(oldKey);
        }
    }

    const std::string encodedKey = std::to_string(key);
    doublyPushFront(cache.order, encodedKey);
    cache.nodes[key] = cache.order.head;
    cache.values[key] = value;
}
