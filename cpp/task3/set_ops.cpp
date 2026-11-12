#include "set_ops.h"

#include <iostream>
#include <vector>

void setUnionOperation(const IntSet& a, const IntSet& b, IntSet& result) {
    for (int value : setElements(&a)) {
        setAdd(&result, value);
    }

    for (int value : setElements(&b)) {
        setAdd(&result, value);
    }
}

void setIntersectionOperation(const IntSet& a, const IntSet& b, IntSet& result) {
    for (int value : setElements(&a)) {
        if (setContains(&b, value)) {
            setAdd(&result, value);
        }
    }
}

void setDifferenceOperation(const IntSet& a, const IntSet& b, IntSet& result) {
    for (int value : setElements(&a)) {
        if (!setContains(&b, value)) {
            setAdd(&result, value);
        }
    }
}

void printSet(const IntSet& set) {
    const std::vector<int> values = setElements(&set);

    std::cout << '{';
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << values[i];
    }
    std::cout << "}\n";
}
