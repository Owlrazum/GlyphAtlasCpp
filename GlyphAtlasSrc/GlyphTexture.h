#pragma once

#include "Tuple.h"
#include "Shelf.h"
#include "Glyph.h"
#include "GlyphKey.h"

#include <vector>
#include <locale>
#include <set>
#include <map>

class GlyphTexture
{
public:
    GlyphTexture(const std::vector<ushort> &shelfDelimitersArg,
                 const std::vector<ushort> &widthDelimitersArg,
                 const Pair &textureDims,
                 ushort idArg)
            : shelfDelimiters(shelfDelimitersArg),
              widthDelimiters(widthDelimitersArg),
              dims(textureDims),
              id(idArg)
    {
        placedGlyphs = std::map<GlyphKey, Glyph>();
        shelves = std::vector<Shelf>();
        freeSpacesForShelves = std::vector<Rect>();
        isInitialized = false;
    }

    void Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);

    [[nodiscard]] std::vector<std::pair<GlyphKey, Glyph>> GetGlyphs() const;
    [[nodiscard]] std::pair<std::vector<Rect>, std::vector<Rect>> GetFreeShelfSlotSpace() const; // the first vector is freeSpace for the shelves

    [[nodiscard]] ushort GetId() const
    { return id; }

private:
    Pair dims{};
    ushort id;

    // array of fonts, max 255 MAX_FONT_COUNT arg
    // glyphId - some structure
    std::map<GlyphKey, Glyph> placedGlyphs;
    std::vector<Shelf> shelves;
    std::vector<Rect> freeSpacesForShelves;

    const std::vector<ushort> shelfDelimiters;
    const std::vector<ushort> widthDelimiters;

    [[nodiscard]] bool ContainsGlyph(const GlyphKey &glyphKey) const;

    bool FitInExistingSpot(Glyph &glyph);

    bool CreateShelf(Glyph &glyph);

    static std::pair<Rect, Rect> SplitFreeSpace(Rect &freeSpace, ushort splitHeight);

    static bool CompareByHeight(const Glyph &lhs, const Glyph &rhs);

    bool isInitialized;

    void Initialize();//std::vector<Rect> &newRects, int idealAreaMultiplier);
};