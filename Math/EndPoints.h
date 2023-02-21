#pragma once

#include "Tuple.h"
#include <vector>

// Traverses pair to merge into, can reduce the size of the container by one in the insertion to the middle case.
// ToRefactor: add EndPoints struct consisting of two positions.
bool MergeIntoIfPossible(Pair &toMerge, std::vector<Pair>& intoContainer);
