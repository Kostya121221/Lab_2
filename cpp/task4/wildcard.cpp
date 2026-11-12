#include "wildcard.h"
#include "array.h"
#include <string>


void appendUtf8Characters(DynamicArray& array, const std::string& text) {
    for (std::size_t i = 0; i < text.size();) {
        const unsigned char lead = static_cast<unsigned char>(text[i]);
        std::size_t length = 1;
        if ((lead & 0xE0) == 0xC0) length = 2;
        else if ((lead & 0xF0) == 0xE0) length = 3;
        else if ((lead & 0xF8) == 0xF0) length = 4;
        if (i + length > text.size()) length = 1;
        arrayAppend(array, text.substr(i, length));
        i += length;
    }
}


std::string at(const DynamicArray& a, std::size_t i) {
    auto value = arrayGet(a, i);
    return value ? *value : std::string();
}

bool wildcardMatch(const std::string& text, const std::string& pattern) {
    DynamicArray textArray, patternArray;
    appendUtf8Characters(textArray, text);
    appendUtf8Characters(patternArray, pattern);

    std::size_t i = 0, j = 0, star = std::string::npos, mark = 0;
    while (i < textArray.size) {
        const std::string tc = at(textArray, i);
        const std::string pc = at(patternArray, j);
        if (j < patternArray.size && (pc == "?" || pc == tc)) {
            ++i; ++j;
        } else if (j < patternArray.size && pc == "*") {
            star = j++;
            mark = i;
        } else if (star != std::string::npos) {
            j = star + 1;
            i = ++mark;
        } else {
            clearArray(textArray);
            clearArray(patternArray);
            return false;
        }
    }
    while (j < patternArray.size && at(patternArray, j) == "*") ++j;
    bool result = j == patternArray.size;
    clearArray(textArray);
    clearArray(patternArray);
    return result;
}
