#include "hash_roman.h"

#include <iostream>
#include <string>


const std::string EMPTY = "0";
const std::string OCCUPIED = "1";
const std::string DELETED = "2";

std::string at(const DynamicArray& array, int index) {
    return arrayGet(array, static_cast<std::size_t>(index)).value_or("");
}

void setAt(DynamicArray& array, int index, const std::string& value) {
    arraySet(array, static_cast<std::size_t>(index), value);
}

unsigned long long firstHashValue(const std::string& key) {
    unsigned long long hash = 14695981039346656037ULL;
    for (unsigned char ch : key) {
        hash ^= ch;
        hash *= 1099511628211ULL;
    }
    return hash;
}

unsigned long long secondHashValue(const std::string& key) {
    unsigned long long hash = 5381;
    for (auto it = key.rbegin(); it != key.rend(); ++it)
        hash = hash * 33 + static_cast<unsigned char>(*it);
    return hash;
}

int firstHash(const std::string& key) {
    return static_cast<int>(firstHashValue(key) % HASH_TABLE_CAPACITY);
}

int secondHash(const std::string& key) {
    return 1 + static_cast<int>(secondHashValue(key) % (HASH_TABLE_CAPACITY - 1));
}

int probeStep(const std::string& key, bool useDoubleHashing) {
    return useDoubleHashing ? secondHash(key) : 1;
}

void putAt(HashTable& table, int index, const std::string& key,
           const std::string& value) {
    setAt(table.keys, index, key);
    setAt(table.values, index, value);
    setAt(table.states, index, OCCUPIED);
}

bool insertAt(HashTable& table, const std::string& key, const std::string& value,
              bool useDoubleHashing, HashStats* stats) {
    int position = firstHash(key);
    const int step = probeStep(key, useDoubleHashing);
    int firstDeleted = -1;

    for (int attempt = 1; attempt <= HASH_TABLE_CAPACITY; ++attempt) {
        const std::string state = at(table.states, position);
        if (stats != nullptr) {
            ++stats->insertionProbes;
            if (static_cast<std::size_t>(attempt) > stats->maxInsertionProbes)
                stats->maxInsertionProbes = static_cast<std::size_t>(attempt);
        }

        if (state == EMPTY) {
            putAt(table, firstDeleted >= 0 ? firstDeleted : position, key, value);
            return true;
        }
        if (state == DELETED) {
            if (firstDeleted < 0) firstDeleted = position;
        } else if (at(table.keys, position) == key) {
            setAt(table.values, position, value);
            return true;
        } else if (stats != nullptr) {
            ++stats->collisions;
        }
        position = (position + step) % HASH_TABLE_CAPACITY;
    }

    if (firstDeleted >= 0) {
        putAt(table, firstDeleted, key, value);
        return true;
    }
    return false;
}

std::optional<std::string> findAt(const HashTable& table, const std::string& key,
                                  bool useDoubleHashing, HashStats* stats) {
    int position = firstHash(key);
    const int step = probeStep(key, useDoubleHashing);
    if (stats != nullptr) ++stats->searchCount;

    for (int attempt = 0; attempt < HASH_TABLE_CAPACITY; ++attempt) {
        const std::string state = at(table.states, position);
        if (stats != nullptr) ++stats->searchProbes;
        if (state == EMPTY) return std::nullopt;
        if (state == OCCUPIED && at(table.keys, position) == key)
            return at(table.values, position);
        position = (position + step) % HASH_TABLE_CAPACITY;
    }
    return std::nullopt;
}

bool eraseAt(HashTable& table, const std::string& key, bool useDoubleHashing) {
    int position = firstHash(key);
    const int step = probeStep(key, useDoubleHashing);

    for (int attempt = 0; attempt < HASH_TABLE_CAPACITY; ++attempt) {
        const std::string state = at(table.states, position);
        if (state == EMPTY) return false;
        if (state == OCCUPIED && at(table.keys, position) == key) {
            setAt(table.keys, position, "");
            setAt(table.values, position, "");
            setAt(table.states, position, DELETED);
            return true;
        }
        position = (position + step) % HASH_TABLE_CAPACITY;
    }
    return false;
}

constexpr int romanValues[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
const char* romanSymbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

bool fillRomanTable(HashTable& table, bool doubleHashing) {
    for (std::size_t i = 0; i < sizeof(romanValues) / sizeof(romanValues[0]); ++i) {
        const std::string key = std::to_string(romanValues[i]);
        const bool inserted = doubleHashing
            ? insertDouble(table, key, romanSymbols[i])
            : insertLinear(table, key, romanSymbols[i]);
        if (!inserted) return false;
    }
    return true;
}

std::string convertUsingTable(int value, const HashTable& table, bool doubleHashing) {
    if (value < 1 || value > 3999) return "ERROR";

    std::string result;
    for (int denomination : romanValues) {
        const std::string key = std::to_string(denomination);
        const std::optional<std::string> symbol = doubleHashing
            ? findDouble(table, key)
            : findLinear(table, key);
        if (!symbol.has_value()) return "ERROR";

        while (value >= denomination) {
            result += *symbol;
            value -= denomination;
        }
    }
    return result;
}

void showMetrics(const char* title, const HashStats& stats, std::size_t keyCount) {
    const double averageInsert = keyCount == 0 ? 0.0
        : static_cast<double>(stats.insertionProbes) / static_cast<double>(keyCount);
    const double averageSearch = stats.searchCount == 0 ? 0.0
        : static_cast<double>(stats.searchProbes) / static_cast<double>(stats.searchCount);
    std::cout << title << ":\n"
              << "  ключей: " << keyCount << '\n'
              << "  коллизий при вставке: " << stats.collisions << '\n'
              << "  среднее число проверок ячеек при вставке: " << averageInsert << '\n'
              << "  максимальное число проверок при одной вставке: " << stats.maxInsertionProbes << '\n'
              << "  среднее число проверок ячеек при успешном поиске: " << averageSearch << '\n';
}


void initHashTable(HashTable& table) {
    destroyHashTable(table);
    for (int i = 0; i < HASH_TABLE_CAPACITY; ++i) {
        arrayAppend(table.keys, "");
        arrayAppend(table.values, "");
        arrayAppend(table.states, EMPTY);
    }
}

void destroyHashTable(HashTable& table) {
    clearArray(table.keys);
    clearArray(table.values);
    clearArray(table.states);
}

bool insertLinear(HashTable& table, const std::string& key,
                  const std::string& value, HashStats* stats) {
    return insertAt(table, key, value, false, stats);
}

bool insertDouble(HashTable& table, const std::string& key,
                  const std::string& value, HashStats* stats) {
    return insertAt(table, key, value, true, stats);
}

std::optional<std::string> findLinear(const HashTable& table,
                                      const std::string& key, HashStats* stats) {
    return findAt(table, key, false, stats);
}

std::optional<std::string> findDouble(const HashTable& table,
                                      const std::string& key, HashStats* stats) {
    return findAt(table, key, true, stats);
}

bool eraseLinear(HashTable& table, const std::string& key) {
    return eraseAt(table, key, false);
}

bool eraseDouble(HashTable& table, const std::string& key) {
    return eraseAt(table, key, true);
}

std::string toRoman(int value) {
    HashTable linearTable{};
    HashTable doubleTable{};
    initHashTable(linearTable);
    initHashTable(doubleTable);

    std::string result = "ERROR";
    if (fillRomanTable(linearTable, false) && fillRomanTable(doubleTable, true)) {
        const std::string linearResult = convertUsingTable(value, linearTable, false);
        const std::string doubleResult = convertUsingTable(value, doubleTable, true);
        if (linearResult == doubleResult) result = linearResult;
    }

    destroyHashTable(linearTable);
    destroyHashTable(doubleTable);
    return result;
}

void printHashAnalysis() {
    constexpr std::size_t keyCount = 50;
    HashTable linearTable{};
    HashTable doubleTable{};
    initHashTable(linearTable);
    initHashTable(doubleTable);
    HashStats linearStats;
    HashStats doubleStats;

    std::string keys[keyCount];
    std::size_t count = 0;
    int candidate = 0;
    while (count < keyCount) {
        const std::string key = "key-" + std::to_string(candidate++);
        if (firstHash(key) == 7) keys[count++] = key;
    }
    for (const std::string& key : keys) {
        insertLinear(linearTable, key, key, &linearStats);
        insertDouble(doubleTable, key, key, &doubleStats);
    }
    for (const std::string& key : keys) {
        findLinear(linearTable, key, &linearStats);
        findDouble(doubleTable, key, &doubleStats);
    }

    std::cout << "Эмпирическое сравнение на " << keyCount
              << " строковых ключах с одинаковым исходным хеш-индексом.\n";
    showMetrics("Линейное пробирование", linearStats, keyCount);
    showMetrics("Двойное хеширование", doubleStats, keyCount);

    destroyHashTable(linearTable);
    destroyHashTable(doubleTable);
}
