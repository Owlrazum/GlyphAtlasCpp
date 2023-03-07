#pragma once

#include "PrimitiveTypes.h"

#include "Glyph.h"
#include "GlyphKey.h"

#include <vector>

std::pair<std::vector<uint16>, std::vector<uint16>> CreateDelimitersByDeltas(
        std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);

void UpdateDelimitersByDeltas(
        std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs,
        std::vector<uint16> &shelfDelimiters,
        std::vector<uint16> &slotDelimiters);
