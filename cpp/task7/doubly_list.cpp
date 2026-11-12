#include "doubly_list.h"

DoublyNode* findDoublyNode(const DoublyList& list, const std::string& value) {
    DoublyNode* node = list.head;
    while (node != nullptr && node->value != value) node = node->next;
    return node;
}

void insertDoublyBetween(DoublyList& list, DoublyNode* previous,
                                DoublyNode* next, const std::string& value) {
    DoublyNode* added = new DoublyNode{value, previous, next};
    if (previous == nullptr) list.head = added;
    else previous->next = added;
    if (next == nullptr) list.tail = added;
    else next->previous = added;
    ++list.size;
}

std::string eraseDoublyNode(DoublyList& list, DoublyNode* node) {
    std::string removed = node->value;
    if (node->previous == nullptr) list.head = node->next;
    else node->previous->next = node->next;
    if (node->next == nullptr) list.tail = node->previous;
    else node->next->previous = node->previous;
    delete node;
    --list.size;
    return removed;
}

std::string doublyPushBack(DoublyList& list, const std::string& value) {
    insertDoublyBetween(list, list.tail, nullptr, value);
    return value;
}

std::string doublyPushFront(DoublyList& list, const std::string& value) {
    insertDoublyBetween(list, nullptr, list.head, value);
    return value;
}

std::optional<std::string> doublyInsertBefore(DoublyList& list, const std::string& target, const std::string& value) {
    DoublyNode* targetNode = findDoublyNode(list, target);
    if (targetNode == nullptr) return std::nullopt;
    insertDoublyBetween(list, targetNode->previous, targetNode, value);
    return value;
}

std::optional<std::string> doublyInsertAfter(DoublyList& list, const std::string& target, const std::string& value) {
    DoublyNode* targetNode = findDoublyNode(list, target);
    if (targetNode == nullptr) return std::nullopt;
    insertDoublyBetween(list, targetNode, targetNode->next, value);
    return value;
}

std::optional<std::string> doublyEraseFront(DoublyList& list) {
    if (list.head == nullptr) return std::nullopt;
    return eraseDoublyNode(list, list.head);
}

std::optional<std::string> doublyEraseBack(DoublyList& list) {
    if (list.tail == nullptr) return std::nullopt;
    return eraseDoublyNode(list, list.tail);
}

std::optional<std::string> doublyEraseValue(DoublyList& list, const std::string& value) {
    DoublyNode* node = findDoublyNode(list, value);
    if (node == nullptr) return std::nullopt;
    return eraseDoublyNode(list, node);
}

std::optional<std::string> doublyEraseBefore(DoublyList& list, const std::string& target) {
    DoublyNode* node = findDoublyNode(list, target);
    if (node == nullptr || node->previous == nullptr) return std::nullopt;
    DoublyNode* removedNode = node->previous;
    return eraseDoublyNode(list, removedNode);
}

std::optional<std::string> doublyEraseAfter(DoublyList& list, const std::string& target) {
    DoublyNode* node = findDoublyNode(list, target);
    if (node == nullptr || node->next == nullptr) return std::nullopt;
    DoublyNode* removedNode = node->next;
    return eraseDoublyNode(list, removedNode);
}

std::optional<std::string> doublyGet(const DoublyList& list, std::size_t index) {
    if (index >= list.size) return std::nullopt;

    DoublyNode* node;
    if (index < list.size / 2) {
        node = list.head;
        while (index-- > 0) node = node->next;
    } else {
        std::size_t steps = list.size - index - 1;
        node = list.tail;
        while (steps-- > 0) node = node->previous;
    }
    return node->value;
}

long long doublyFind(const DoublyList& list, const std::string& value) {
    long long index = 0;
    for (DoublyNode* node = list.head; node != nullptr; node = node->next, ++index)
        if (node->value == value) return index;
    return -1;
}

void doublyMoveToFront(DoublyList& list, DoublyNode* node) {
    if (node == nullptr || node == list.head) return;

    if (node->previous != nullptr) node->previous->next = node->next;
    if (node->next != nullptr) node->next->previous = node->previous;
    else list.tail = node->previous;

    node->previous = nullptr;
    node->next = list.head;
    if (list.head != nullptr) list.head->previous = node;
    list.head = node;
    if (list.tail == nullptr) list.tail = node;
}

void clearDoublyList(DoublyList& list) {
    while (list.head != nullptr) eraseDoublyNode(list, list.head);
}
