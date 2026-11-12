#include "set.h"

#include <cstdint>
#include <string>


constexpr std::size_t INITIAL_CAPACITY = 101;

std::uint64_t hashInteger(int value) {
    std::uint64_t hash = static_cast<std::uint64_t>(static_cast<std::int64_t>(value));
    hash += 0x9e3779b97f4a7c15ULL;
    hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9ULL;
    hash = (hash ^ (hash >> 27)) * 0x94d049bb133111ebULL;
    return hash ^ (hash >> 31);
}

std::size_t bucketIndex(int value, std::size_t capacity) {
    return static_cast<std::size_t>(hashInteger(value) % capacity);
}

bool isPrime(std::size_t value) {
    if (value < 2) return false;
    if (value % 2 == 0) return value == 2;

    for (std::size_t divisor = 3; divisor <= value / divisor; divisor += 2) {
        if (value % divisor == 0) return false;
    }
    return true;
}

std::size_t nextCapacity(std::size_t current) {
    std::size_t candidate = current * 2 + 1;
    while (!isPrime(candidate)) {
        candidate += 2;
    }
    return candidate;
}

void rehash(IntSet* set) {
    const std::size_t newCapacity = nextCapacity(set->capacity);
    SinglyList* newBuckets = new SinglyList[newCapacity]{};

    for (std::size_t i = 0; i < set->capacity; ++i) {
        SinglyNode* current = set->buckets[i].head;
        while (current != nullptr) {
            SinglyNode* next = current->next;
            int value = std::stoi(current->value);
            std::size_t index = bucketIndex(value, newCapacity);

            current->next = newBuckets[index].head;
            newBuckets[index].head = current;
            current = next;
        }
    }

    delete[] set->buckets;
    set->buckets = newBuckets;
    set->capacity = newCapacity;
}


void setInit(IntSet* set) {
    if (set == nullptr || set->buckets != nullptr) return;

    set->capacity = INITIAL_CAPACITY;
    set->size = 0;
    set->buckets = new SinglyList[set->capacity]{};
}

bool setContains(const IntSet* set, int value) {
    if (set == nullptr || set->buckets == nullptr || set->capacity == 0) {
        return false;
    }

    const std::size_t index = bucketIndex(value, set->capacity);
    return singlyFind(set->buckets[index], std::to_string(value)) >= 0;
}

void setAdd(IntSet* set, int value) {
    if (set == nullptr) return;
    if (set->buckets == nullptr) setInit(set);

    const std::string key = std::to_string(value);
    std::size_t index = bucketIndex(value, set->capacity);

    if (singlyFind(set->buckets[index], key) >= 0) return;

    if ((set->size + 1) * 4 > set->capacity * 3) {
        rehash(set);
        index = bucketIndex(value, set->capacity);
    }

    singlyPushFront(set->buckets[index], key);
    ++set->size;
}

bool setRemove(IntSet* set, int value) {
    if (set == nullptr || set->buckets == nullptr || set->capacity == 0) {
        return false;
    }

    const std::size_t index = bucketIndex(value, set->capacity);
    if (!singlyEraseValue(set->buckets[index], std::to_string(value))) {
        return false;
    }

    --set->size;
    return true;
}

std::vector<int> setElements(const IntSet* set) {
    std::vector<int> result;
    if (set == nullptr || set->buckets == nullptr) return result;

    result.reserve(set->size);
    for (std::size_t i = 0; i < set->capacity; ++i) {
        for (SinglyNode* current = set->buckets[i].head;
             current != nullptr; current = current->next) {
            result.push_back(std::stoi(current->value));
        }
    }
    return result;
}

void setDestroy(IntSet* set) {
    if (set == nullptr) return;

    if (set->buckets != nullptr) {
        for (std::size_t i = 0; i < set->capacity; ++i) {
            clearSinglyList(set->buckets[i]);
        }
        delete[] set->buckets;
    }

    set->buckets = nullptr;
    set->capacity = 0;
    set->size = 0;
}
