#pragma once

#include <cstddef>
#include <string>
#include <optional>

struct DynamicArray {
    std::string* items = nullptr;
    std::size_t size = 0;
    std::size_t capacity = 0;
};

std::string arrayAppend(DynamicArray& array, const std::string& value);
std::optional<std::string> arrayInsert(DynamicArray& array, std::size_t index, const std::string& value);
std::optional<std::string> arrayGet(const DynamicArray& array, std::size_t index);
std::optional<std::string> arraySet(DynamicArray& array, std::size_t index, const std::string& value);
std::optional<std::string> arrayErase(DynamicArray& array, std::size_t index);
void clearArray(DynamicArray& array);
