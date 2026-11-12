#include "huffman.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>


struct HuffmanNode {
    int symbol;
    unsigned long long frequency;
    int minSymbol;
    unsigned long long order;
    HuffmanNode* left;
    HuffmanNode* right;

};

bool huffmanIsLeaf(const HuffmanNode* node) {
    return node != nullptr && node->left == nullptr && node->right == nullptr;
}

void destroyTree(HuffmanNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

bool lessNode(const HuffmanNode* a, const HuffmanNode* b) {
    if (a->frequency != b->frequency) return a->frequency < b->frequency;
    if (a->minSymbol != b->minSymbol) return a->minSymbol < b->minSymbol;
    return a->order < b->order;
}

void makeCodebook(HuffmanNode* node, const std::string& prefix,
                  std::string (&codes)[256], DynamicArray& codebook) {
    if (!node) return;
    if (huffmanIsLeaf(node)) {
        const std::string code = prefix.empty() ? "0" : prefix;
        codes[node->symbol] = code;
        arrayAppend(codebook, std::to_string(node->symbol) + "|" + code);
        return;
    }
    makeCodebook(node->left, prefix + "0", codes, codebook);
    makeCodebook(node->right, prefix + "1", codes, codebook);
}

void serializeTree(const HuffmanNode* node, std::ostringstream& output, bool& firstToken) {
    if (!node) return;
    if (!firstToken) output << ' ';
    firstToken = false;
    if (huffmanIsLeaf(node)) {
        output << "L:" << node->symbol;
        return;
    }
    output << 'I';
    serializeTree(node->left, output, firstToken);
    serializeTree(node->right, output, firstToken);
}

HuffmanNode* buildTreeFromTokens(const std::string tokens[], std::size_t count,
                                 std::size_t& position, bool& ok) {
    if (!ok || position >= count) {
        ok = false;
        return nullptr;
    }
    const std::string token = tokens[position++];
    if (token == "I") {
        HuffmanNode* node = new HuffmanNode{-1, 0, -1, 0, nullptr, nullptr};
        node->left = buildTreeFromTokens(tokens, count, position, ok);
        node->right = buildTreeFromTokens(tokens, count, position, ok);
        if (!ok || !node->left || !node->right) {
            destroyTree(node);
            ok = false;
            return nullptr;
        }
        return node;
    }
    if (token.rfind("L:", 0) == 0) {
        try {
            std::size_t used = 0;
            const int symbol = std::stoi(token.substr(2), &used);
            if (used != token.size() - 2 || symbol < 0 || symbol > 255) {
                ok = false;
                return nullptr;
            }
            return new HuffmanNode{symbol, 0, symbol, 0, nullptr, nullptr};
        } catch (...) {
            ok = false;
            return nullptr;
        }
    }
    ok = false;
    return nullptr;
}


std::string huffmanEncode(const std::string& text, std::string& serializedTree,
                          DynamicArray& codebook) {
    clearArray(codebook);
    serializedTree.clear();
    if (text.empty()) {
        serializedTree = "EMPTY";
        return "";
    }

    unsigned long long frequencies[256] = {};
    for (unsigned char byte : text) ++frequencies[byte];

    std::size_t uniqueCount = 0;
    for (unsigned long long frequency : frequencies)
        if (frequency != 0) ++uniqueCount;

    const std::size_t capacity = uniqueCount * 2 - 1;
    HuffmanNode** forest = new HuffmanNode*[capacity];
    std::size_t forestSize = 0;
    unsigned long long nextOrder = 0;
    try {
        for (int symbol = 0; symbol < 256; ++symbol) {
            if (frequencies[symbol] == 0) continue;
            forest[forestSize++] = new HuffmanNode{
                symbol, frequencies[symbol], symbol, nextOrder++, nullptr, nullptr};
        }

        while (forestSize > 1) {
            std::size_t first = 0;
            for (std::size_t i = 1; i < forestSize; ++i)
                if (lessNode(forest[i], forest[first])) first = i;
            HuffmanNode* left = forest[first];
            forest[first] = forest[--forestSize];

            std::size_t second = 0;
            for (std::size_t i = 1; i < forestSize; ++i)
                if (lessNode(forest[i], forest[second])) second = i;
            HuffmanNode* right = forest[second];
            forest[second] = forest[--forestSize];

            if (left->frequency > std::numeric_limits<unsigned long long>::max() - right->frequency) {
                forest[forestSize++] = left;
                forest[forestSize++] = right;
                throw std::overflow_error("frequency overflow");
            }
            HuffmanNode* parent = nullptr;
            try {
                parent = new HuffmanNode{
                    -1, left->frequency + right->frequency,
                    std::min(left->minSymbol, right->minSymbol), nextOrder++, left, right};
            } catch (...) {
                forest[forestSize++] = left;
                forest[forestSize++] = right;
                throw;
            }
            forest[forestSize++] = parent;
        }

        HuffmanNode* root = forest[0];
        std::string codes[256];
        makeCodebook(root, "", codes, codebook);
        std::ostringstream treeOutput;
        bool firstToken = true;
        serializeTree(root, treeOutput, firstToken);
        serializedTree = treeOutput.str();

        std::string encoded;
        for (unsigned char byte : text) encoded += codes[byte];
        destroyTree(root);
        delete[] forest;
        return encoded;
    } catch (...) {
        for (std::size_t i = 0; i < forestSize; ++i) destroyTree(forest[i]);
        delete[] forest;
        clearArray(codebook);
        throw;
    }
}

std::string huffmanDecode(const std::string& bits, const std::string& serializedTree,
                          bool& ok) {
    ok = false;
    std::string treeText = serializedTree;
    const auto trim = [](const std::string& value) {
        std::size_t first = 0;
        while (first < value.size() &&
               std::isspace(static_cast<unsigned char>(value[first]))) ++first;
        std::size_t last = value.size();
        while (last > first &&
               std::isspace(static_cast<unsigned char>(value[last - 1]))) --last;
        return value.substr(first, last - first);
    };
    treeText = trim(treeText);
    if (treeText.rfind("TREE ", 0) == 0) treeText = trim(treeText.substr(5));
    if (treeText == "EMPTY") {
        ok = bits.empty();
        return "";
    }

    std::istringstream input(treeText);
    std::string token;
    std::string tokens[512];
    std::size_t count = 0;
    while (input >> token) {
        if (count == 512) return "";
        tokens[count++] = token;
    }
    if (count > 0 && tokens[0] == "TREE") {
        for (std::size_t i = 1; i < count; ++i) tokens[i - 1] = tokens[i];
        --count;
    }
    if (count == 0) return "";

    std::size_t position = 0;
    bool parsed = true;
    HuffmanNode* root = buildTreeFromTokens(tokens, count, position, parsed);
    if (!parsed || !root || position != count) {
        destroyTree(root);
        return "";
    }

    std::string decoded;
    HuffmanNode* current = root;
    if (huffmanIsLeaf(root)) {
        for (char bit : bits) {
            if (bit != '0') {
                destroyTree(root);
                return "";
            }
            decoded.push_back(static_cast<char>(root->symbol));
        }
    } else {
        for (char bit : bits) {
            if (bit == '0') current = current->left;
            else if (bit == '1') current = current->right;
            else {
                destroyTree(root);
                return "";
            }
            if (!current) {
                destroyTree(root);
                return "";
            }
            if (huffmanIsLeaf(current)) {
                decoded.push_back(static_cast<char>(current->symbol));
                current = root;
            }
        }
        if (current != root) {
            destroyTree(root);
            return "";
        }
    }

    destroyTree(root);
    ok = true;
    return decoded;
}
