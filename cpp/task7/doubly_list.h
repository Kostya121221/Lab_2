#pragma once

#include <cstddef>
#include <string>
#include <optional>

struct DoublyNode {
    std::string value;
    DoublyNode* previous = nullptr;
    DoublyNode* next = nullptr;
};

struct DoublyList {
    DoublyNode* head = nullptr;
    DoublyNode* tail = nullptr;
    std::size_t size = 0;
};

std::string doublyPushBack(DoublyList& list, const std::string& value);
std::string doublyPushFront(DoublyList& list, const std::string& value);
std::optional<std::string> doublyInsertBefore(DoublyList& list, const std::string& target, const std::string& value);
std::optional<std::string> doublyInsertAfter(DoublyList& list, const std::string& target, const std::string& value);
std::optional<std::string> doublyEraseFront(DoublyList& list);
std::optional<std::string> doublyEraseBack(DoublyList& list);
std::optional<std::string> doublyEraseValue(DoublyList& list, const std::string& value);
std::optional<std::string> doublyEraseBefore(DoublyList& list, const std::string& target);
std::optional<std::string> doublyEraseAfter(DoublyList& list, const std::string& target);
void doublyMoveToFront(DoublyList& list, DoublyNode* node);
std::optional<std::string> doublyGet(const DoublyList& list, std::size_t index);
long long doublyFind(const DoublyList& list, const std::string& value);
void clearDoublyList(DoublyList& list);
