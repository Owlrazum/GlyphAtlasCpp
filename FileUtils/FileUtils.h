#pragma once

#include "Rect.h"
#include "GlyphKey.h"
#include "Glyph.h"

#include <vector>

std::string GetFontPath(machine fontId);
std::string GetTestGlyphKeysPath(machine testNumber);
std::string GetSvgTexturePath(machine textureId);

// writes rndFontIndex and rnd latin character
void WriteTestGlyphKeys(
        std::ofstream &out,
        machine fontCount,
        machine keysCountInPass);

std::vector<std::vector<std::pair<GlyphKey, Glyph>>> ReadGlyphKeysByLine(const std::string &path);

// obsoletes
//void ReadGlyphRects(std::vector<std::pair<GlyphKey, Glyph>> &glyphs);
//void WriteTestFontRects(const std::string& path, const std::vector<Rect>& rects);
//std::string GetTestFontPath(machine fontId);
