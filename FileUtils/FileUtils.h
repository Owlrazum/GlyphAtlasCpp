#pragma once

#include "Rect.h"
#include "GlyphKey.h"
#include "Glyph.h"

#include <vector>

std::string GetFontPath(int fontId);
std::string GetDataPath(int testNumber);
std::string GetTexturePath(int textureId);
void WriteRects(const std::string& path, const std::vector<Rect>& rects);
void GenerateAndWriteGlyphKeys(
        const std::string& path,
        int fontCount,
        const std::vector<int>& glyphCounts);
std::vector<GlyphKey> ReadGlyphKeys(const std::string& path);
std::vector<std::pair<GlyphKey, Glyph>> ReadGlyphs(const std::vector<GlyphKey>& keys);
std::vector<std::vector<GlyphKey>> ReadGlyphKeysByLine(const std::string &path);
