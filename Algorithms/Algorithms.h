#pragma once

#include "PrimitiveTypes.h"

#include "Glyph.h"
#include "GlyphKey.h"

#include <vector>
#include <map>

std::pair<std::vector<uint16>, std::vector<uint16>> CreateDelimitersByDeltas(
        std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);

void UpdateDelimitersByDeltas(
        std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs,
        std::vector<uint16> &shelfDelimiters,
        std::vector<uint16> &slotDelimiters);

// Traverses pair to merge into, can reduce the size of the container by one in the insertion to the middle case.
bool MergeIntoIfPossible(uint16_2 &toMerge, std::vector<uint16_2>& intoContainer);
void CheckContainerIntegrity(std::vector<uint16_2>& container, uint16 growTarget);