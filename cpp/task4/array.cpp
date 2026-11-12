#include "array.h"

void growArray(DynamicArray& array, std::size_t required) {
    if (array.capacity >= required) return;

    std::size_t capacity = array.capacity == 0 ? 4 : array.capacity;
    while (capacity < required) capacity *= 2;

    std::string* items = new std::string[capacity];
    for (std::size_t i = 0; i < array.size; ++i) items[i] = array.items[i];
    delete[] array.items;
    array.items = items;
    array.capacity = capacity;
}

std::string arrayAppend(DynamicArray& array, const std::string& value) {
    growArray(array, array.size + 1);
    array.items[array.size++] = value;
    return value;
}

std::optional<std::string> arrayInsert(DynamicArray& array, std::size_t index, const std::string& value) {
    if (index > array.size) return std::nullopt;
    growArray(array, array.size + 1);
    for (std::size_t i = array.size; i > index; --i) array.items[i] = array.items[i - 1];
    array.items[index] = value;
    ++array.size;
    return value;
}

std::optional<std::string> arrayGet(const DynamicArray& array, std::size_t index) {
    if (index >= array.size) return std::nullopt;
    return array.items[index];
}

std::optional<std::string> arraySet(DynamicArray& array, std::size_t index, const std::string& value) {
    if (index >= array.size) return std::nullopt;
    array.items[index] = value;
    return value;
}

std::optional<std::string> arrayErase(DynamicArray& array, std::size_t index) {
    if (index >= array.size) return std::nullopt;
    std::string removed = array.items[index];
    for (std::size_t i = index; i + 1 < array.size; ++i) array.items[i] = array.items[i + 1];
    --array.size;
    return removed;
}

void clearArray(DynamicArray& array) {
    delete[] array.items;
    array.items = nullptr;
    array.size = 0;
    array.capacity = 0;
}
