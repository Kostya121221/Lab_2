#include "hash_roman.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--analysis") {
        printHashAnalysis();
        return 0;
    }

    int value;
    if (!(std::cin >> value)) return 0;
    std::cout << toRoman(value) << '\n';
    return 0;
}
