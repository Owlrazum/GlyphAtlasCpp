#pragma once

#include "PrimitiveTypes.h"

#include "Glyph.h"
#include "GlyphKey.h"

#include <vector>
#include <map>

// https://stackoverflow.com/questions/15579623/find-set-difference-between-set-and-map-keys
class KeyLess
{
public:

    template< typename KeyType >
    static KeyType GetKey( const KeyType& k )
    {
        return k;
    }

    template< typename KeyType, typename ValueType >
    static KeyType GetKey( std::pair< const KeyType, ValueType > const& p )
    {
        return p.first;
    }

    template< typename L, typename R >
    bool operator()( const L& l, const R& r ) const
    {
        return GetKey( l ) < GetKey( r );
    }
};

std::pair<std::vector<uint16>, std::vector<uint16>> CreateDelimitersByDeltas(
        std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs);

void UpdateDelimitersByDeltas(
        std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs,
        std::vector<uint16> &shelfDelimiters,
        std::vector<uint16> &slotDelimiters);

uint16 ToLength(const uint16_2& endPoints);
uint16_2 ToEndPoints(uint16 start, uint16 length);

// Traverses pair to merge into, can reduce the size of the container by one in the insertion to the middle case.
bool MergeIntoIfPossible(uint16_2 &toMerge, std::vector<uint16_2>& intoContainer);
void CheckContainerIntegrity(std::vector<uint16_2>& container, uint16 growTarget);