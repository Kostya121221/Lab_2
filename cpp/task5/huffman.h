#pragma once

#include "array.h"
#include <string>

std::string huffmanEncode(const std::string& text, std::string& serializedTree,
                          DynamicArray& codebook);
std::string huffmanDecode(const std::string& bits, const std::string& serializedTree,
                          bool& ok);
