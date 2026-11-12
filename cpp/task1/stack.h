#pragma once

#include <cstddef>
#include <string>
#include <optional>

struct StackNode {
    std::string value;
    StackNode* next = nullptr;
};

struct StackData {
    StackNode* top = nullptr;
    std::size_t size = 0;
};

std::string stackPush(StackData& stack, const std::string& value);
std::optional<std::string> stackPop(StackData& stack);
std::optional<std::string> stackPeek(const StackData& stack);
void clearStack(StackData& stack);
