#pragma once

#include "array.h"
#include <cstddef>
#include <optional>
#include <string>

constexpr int HASH_TABLE_CAPACITY = 101;

struct HashStats {
    std::size_t insertionProbes = 0;
    std::size_t collisions = 0;
    std::size_t maxInsertionProbes = 0;
    std::size_t searchProbes = 0;
    std::size_t searchCount = 0;
};

struct HashTable {
    DynamicArray keys;
    DynamicArray values;
    DynamicArray states;
};

void initHashTable(HashTable& table);
void destroyHashTable(HashTable& table);

bool insertLinear(HashTable& table, const std::string& key,
                  const std::string& value, HashStats* stats = nullptr);
bool insertDouble(HashTable& table, const std::string& key,
                  const std::string& value, HashStats* stats = nullptr);
std::optional<std::string> findLinear(const HashTable& table,
                                      const std::string& key, HashStats* stats = nullptr);
std::optional<std::string> findDouble(const HashTable& table,
                                      const std::string& key, HashStats* stats = nullptr);
bool eraseLinear(HashTable& table, const std::string& key);
bool eraseDouble(HashTable& table, const std::string& key);

std::string toRoman(int value);
void printHashAnalysis();
