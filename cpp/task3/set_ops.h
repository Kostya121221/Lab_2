#pragma once

#include "../task2/set.h"

void setUnionOperation(const IntSet& a, const IntSet& b, IntSet& result);
void setIntersectionOperation(const IntSet& a, const IntSet& b, IntSet& result);
void setDifferenceOperation(const IntSet& a, const IntSet& b, IntSet& result);
void printSet(const IntSet& set);
