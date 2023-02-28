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
std::vector<std::pair<GlyphKey, Glyph>> ReadGlyphs(const std::vector<GlyphKey>& keys);
std::vector<std::vector<GlyphKey>> ReadGlyphKeysByLine(const std::string &path);


std::string GetFontPath(int fontId);