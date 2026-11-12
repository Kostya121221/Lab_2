#include "huffman.h"

#include <iostream>
#include <string>

std::string readLine() {
    std::string line;
    std::getline(std::cin, line);
    if (!line.empty() && line.back() == '\r') line.pop_back();
    return line;
}

int main() {
    const std::string mode = readLine();
    if (mode == "E" || mode == "ENCODE" || mode == "encode") {
        const std::string text = readLine();
        DynamicArray codebook;
        std::string tree;
        try {
            const std::string bits = huffmanEncode(text, tree, codebook);
            std::cout << "CODE " << bits << '\n';
            std::cout << "TREE " << tree << '\n';
            std::cout << "TABLE\n";
            for (std::size_t i = 0; i < codebook.size; ++i) {
                const auto entry = arrayGet(codebook, i);
                if (!entry) continue;
                const std::size_t separator = entry->find('|');
                if (separator != std::string::npos)
                    std::cout << entry->substr(0, separator) << ' '
                              << entry->substr(separator + 1) << '\n';
            }
            std::cout << "ENDTABLE\n";
            bool decodedOk = false;
            const std::string decoded = huffmanDecode(bits, tree, decodedOk);
            if (decodedOk) std::cout << "DECODED " << decoded << '\n';
            else std::cout << "ERROR\n";
        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << '\n';
            clearArray(codebook);
            return 1;
        }
        clearArray(codebook);
        return 0;
    }

    if (mode == "D" || mode == "DECODE" || mode == "decode") {
        const std::string bits = readLine();
        const std::string tree = readLine();
        bool ok = false;
        const std::string decoded = huffmanDecode(bits, tree, ok);
        if (!ok) {
            std::cout << "ERROR\n";
            return 1;
        }
        std::cout << "DECODED " << decoded << '\n';
        return 0;
    }

    std::cerr << "E Enter <...> - что-то закодировать, D Enter <Срока начинающаяся на TREE> Enter <...> - что-то раскодировать\n";
    return 1;
}
