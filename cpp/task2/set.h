#pragma once

#include "../common/singly_list.h"

#include <cstddef>
#include <vector>

struct IntSet {
    SinglyList* buckets = nullptr;
    std::size_t capacity = 0;
    std::size_t size = 0;
};

void setInit(IntSet* set);
void setAdd(IntSet* set, int value);
bool setRemove(IntSet* set, int value);
bool setContains(const IntSet* set, int value);
std::vector<int> setElements(const IntSet* set);
void setDestroy(IntSet* set);
