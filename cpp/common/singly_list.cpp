#include "singly_list.h"

std::string singlyPushFront(SinglyList& list, const std::string& value) {
    list.head = new SinglyNode{value, list.head};
    return value;
}

std::optional<std::string> singlyEraseValue(SinglyList& list, const std::string& value) {
    SinglyNode* previous = nullptr;
    SinglyNode* current = list.head;

    while (current != nullptr && current->value != value) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        return std::nullopt;
    }

    if (previous == nullptr) {
        list.head = current->next;
    } else {
        previous->next = current->next;
    }

    std::string removed = current->value;
    delete current;
    return removed;
}

long long singlyFind(const SinglyList& list, const std::string& value) {
    long long index = 0;
    for (SinglyNode* current = list.head; current != nullptr;
         current = current->next, ++index) {
        if (current->value == value) {
            return index;
        }
    }
    return -1;
}

void clearSinglyList(SinglyList& list) {
    while (list.head != nullptr) {
        SinglyNode* removed = list.head;
        list.head = removed->next;
        delete removed;
    }
}
