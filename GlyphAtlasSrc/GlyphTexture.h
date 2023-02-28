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
              id(idArg),
              placedGlyphs(),
              previouslyPlacedGlyphs(),
              currentlyPlacedGlyphs(),
              shelves(),
              freeShelves()
    {
        freeShelves.emplace_back(0, dims.y - 1);
    }

    void Update(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);
    void RemoveUnused();

    [[nodiscard]] bool ContainsGlyph(const GlyphKey &key) const
    {
        return placedGlyphs.find(key) != placedGlyphs.end();
    }

    void MarkGlyph(const GlyphKey &key)
    {
        currentlyPlacedGlyphs.insert(key);
    }

    bool GetGlyph(GlyphKey key, Glyph& glyph); // returns whether the glyph was found

    int GetGlyphCount()
    {
        return static_cast<int>(placedGlyphs.size());
    }

    [[nodiscard]] std::vector<std::pair<GlyphKey, Glyph>> GetGlyphs() const;
    [[nodiscard]] std::pair<std::vector<Rect>, std::vector<Rect>> GetFreeShelfSlotSpace() const; // the first vector is freeSpace for the shelves

    [[nodiscard]] ushort GetId() const
    { return id; }

    bool Step(std::pair<GlyphKey, Glyph> toPlace);
private:
    Pair dims;
    ushort id;

    std::vector<Shelf> shelves;
    std::vector<Pair> freeShelves; // Pair.x - yMin pos, Pair.y - yMax pos

    std::map<GlyphKey, Glyph> placedGlyphs; // we need prev and cur, becuase placedGlyph is basically a mixture of both.
    std::set<GlyphKey> previouslyPlacedGlyphs;
    std::set<GlyphKey> currentlyPlacedGlyphs; // in other words, glyphs that were placed during this iteration.

    const std::vector<ushort> shelfDelimiters;
    const std::vector<ushort> widthDelimiters;

    bool FitInExistingSpot(std::pair<GlyphKey, Glyph> &glyph, ushort slotWidth);

    bool CreateShelf(std::pair<GlyphKey, Glyph> &glyph, ushort slotWidth);

    static void SplitFreeSpace(Pair &freeShelf, ushort splitHeight);
    void RemoveShelf(std::vector<Shelf>::iterator shelfToRemove);
    void ClaimFreeShelf(Pair &freeShelf);
};