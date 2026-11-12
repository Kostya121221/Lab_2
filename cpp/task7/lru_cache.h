#pragma once

#include <unordered_map>
#include "doubly_list.h"

struct LRUCache {
    int capacity = 0;
    DoublyList order;
    std::unordered_map<int, DoublyNode*> nodes;
    std::unordered_map<int, int> values;
};

void initLRUCache(LRUCache& cache, int capacity);
void clearLRUCache(LRUCache& cache);
int lruGet(LRUCache& cache, int key);
void lruSet(LRUCache& cache, int key, int value);
