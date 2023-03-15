#pragma once

#include "Rect.h"
#include "GlyphKey.h"
#include "Glyph.h"

#include <vector>

std::string GetFontPath(machine fontId);
std::string GetTestGlyphKeysPath(machine testNumber);

// writes rndFontIndex and rnd latin character
void WriteGlyphTestData(
        std::ofstream &out,
        machine fontCount,
        machine keysCountInPass);

std::vector<std::vector<std::pair<FontKey, GlyphKey>>> ReadGlyphTestData(const std::string &path);
