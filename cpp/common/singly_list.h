#pragma once

#include <optional>
#include <string>

struct SinglyNode {
    std::string value;
    SinglyNode* next = nullptr;
};

struct SinglyList {
    SinglyNode* head = nullptr;
};

std::string singlyPushFront(SinglyList& list, const std::string& value);
std::optional<std::string> singlyEraseValue(SinglyList& list, const std::string& value);
long long singlyFind(const SinglyList& list, const std::string& value);
void clearSinglyList(SinglyList& list);
