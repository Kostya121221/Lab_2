#include "set_ops.h"

#include <iostream>

int main() {
    int countA;
    if (!(std::cin >> countA) || countA < 0) return 0;

    IntSet a{}, b{}, united{}, intersected{}, difference{};
    for (int i = 0; i < countA; ++i) {
        int value;
        if (!(std::cin >> value)) {
            setDestroy(&a);
            setDestroy(&b);
            setDestroy(&united);
            setDestroy(&intersected);
            setDestroy(&difference);
            return 0;
        }
        setAdd(&a, value);
    }

    int countB;
    if (!(std::cin >> countB) || countB < 0) {
        setDestroy(&a);
        setDestroy(&b);
        setDestroy(&united);
        setDestroy(&intersected);
        setDestroy(&difference);
        return 0;
    }
    for (int i = 0; i < countB; ++i) {
        int value;
        if (!(std::cin >> value)) {
            setDestroy(&a);
            setDestroy(&b);
            setDestroy(&united);
            setDestroy(&intersected);
            setDestroy(&difference);
            return 0;
        }
        setAdd(&b, value);
    }

    setUnionOperation(a, b, united);
    setIntersectionOperation(a, b, intersected);
    setDifferenceOperation(a, b, difference);
    printSet(united);
    printSet(intersected);
    printSet(difference);

    setDestroy(&a);
    setDestroy(&b);
    setDestroy(&united);
    setDestroy(&intersected);
    setDestroy(&difference);
    return 0;
}
