#pragma once

#include "Rect.h"
#include "GlyphKey.h"
#include "Glyph.h"

#include <vector>

std::string GetTestFontPath(int fontId);
std::string GetTestGlyphKeysPath(int testNumber);
std::string GetSvgTexturePath(int textureId);

void WriteTestFontRects(const std::string& path, const std::vector<Rect>& rects);
void WriteTestGlyphKeys(
        std::ofstream &out,
        int fontCount,
        int glyphCountInFont,
        int keysCountInPass);

void ReadGlyphs(std::vector<std::pair<GlyphKey, Glyph>> &glyphs);
std::vector<std::vector<std::pair<GlyphKey, Glyph>>> ReadGlyphKeysByLine(const std::string &path);

std::string GetFontPath(int fontId);