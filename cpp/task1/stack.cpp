#include "stack.h"

std::string stackPush(StackData& stack, const std::string& value) {
    stack.top = new StackNode{value, stack.top};
    ++stack.size;
    return value;
}

std::optional<std::string> stackPop(StackData& stack) {
    if (stack.top == nullptr) return std::nullopt;
    StackNode* node = stack.top;
    std::string value = node->value;
    stack.top = node->next;
    delete node;
    --stack.size;
    return value;
}

std::optional<std::string> stackPeek(const StackData& stack) {
    if (stack.top == nullptr) return std::nullopt;
    return stack.top->value;
}

void clearStack(StackData& stack) {
    while (stack.top != nullptr) {
        StackNode* node = stack.top;
        stack.top = node->next;
        delete node;
    }
    stack.size = 0;
}
