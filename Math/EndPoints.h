#pragma once

#include "PrimitiveTypes.h"
#include <vector>

// Traverses pair to merge into, can reduce the size of the container by one in the insertion to the middle case.
// ToRefactor: add EndPoints struct consisting of two positions.
bool MergeIntoIfPossible(uint16_2 &toMerge, std::vector<uint16_2>& intoContainer);
