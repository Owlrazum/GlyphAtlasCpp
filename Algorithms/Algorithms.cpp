#include "Algorithms.h"

#include <set>

static bool CompareByHeight(const std::pair<GlyphKey, Glyph> &lhs, const std::pair<GlyphKey, Glyph> &rhs)
{
    return lhs.second.rect.h < rhs.second.rect.h;
}

static bool CompareByWidth(const std::pair<GlyphKey, Glyph> &lhs, const std::pair<GlyphKey, Glyph> &rhs)
{
    return lhs.second.rect.w < rhs.second.rect.w;
}

static bool CompareByHeightReverse(const std::pair<GlyphKey, Glyph> &lhs, const std::pair<GlyphKey, Glyph> &rhs)
{
    return lhs.second.rect.h > rhs.second.rect.h;
}

static bool CompareByWidthReverse(const std::pair<GlyphKey, Glyph> &lhs, const std::pair<GlyphKey, Glyph> &rhs)
{
    return lhs.second.rect.w > rhs.second.rect.w;
}

static uint16 GetMaxWidth(const std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    return std::max_element(updateGlyphs.begin(), updateGlyphs.end(), CompareByWidth)->second.rect.w;
}
static uint16 GetMaxHeight(const std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    return std::max_element(updateGlyphs.begin(), updateGlyphs.end(), CompareByHeight)->second.rect.h;
}

std::pair<std::vector<uint16>, std::vector<uint16>> CreateDelimitersByDeltas(
        std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs)
{
    std::vector<uint16> shelfDelimiters, slotDelimiters;

    std::sort(updateGlyphs.begin(), updateGlyphs.end(), CompareByWidth);

    float baseValue = updateGlyphs.begin()->second.rect.w;
    float prevDelta = std::max(5.0f, static_cast<float>((updateGlyphs.begin() + 1)->second.rect.w) - baseValue);

    bool hasPassedThreshold = false;
    float passedThresholdValue = -1;
    for (auto i = updateGlyphs.begin() + 2; i < updateGlyphs.end() - 2; i++)
    {
        if (hasPassedThreshold)
        {
            if (i->second.rect.w - passedThresholdValue > 3)
            {
                slotDelimiters.emplace_back((i - 1)->second.rect.w);
                baseValue = i->second.rect.w;
                i++;
                prevDelta = std::max(5.0f, i->second.rect.w - baseValue);
                hasPassedThreshold = false;
            }
            continue;
        }

        auto value = i->second.rect.w;
        float delta = value - baseValue;
        if (delta / prevDelta >= 1.1f)
        {
            hasPassedThreshold = true;
            passedThresholdValue = value;
        }
    }

    slotDelimiters.emplace_back((updateGlyphs.end() - 1)->second.rect.w);

    std::sort(updateGlyphs.begin(), updateGlyphs.end(), CompareByHeight);

    baseValue = updateGlyphs.begin()->second.rect.h;
    prevDelta = std::max(5.0f, (updateGlyphs.begin() + 1)->second.rect.h - baseValue);

    hasPassedThreshold = false;
    for (auto i = updateGlyphs.begin() + 2; i < updateGlyphs.end() - 2; i++)
    {
        if (hasPassedThreshold)
        {
            if (i->second.rect.h - passedThresholdValue > 3)
            {
                shelfDelimiters.emplace_back((i - 1)->second.rect.h);
                baseValue = i->second.rect.h;
                i++;
                prevDelta = std::max(5.0f, i->second.rect.h - baseValue);
                hasPassedThreshold = false;
            }
            continue;
        }

        auto value = i->second.rect.h;
        float delta = value - baseValue;
        if (delta / prevDelta >= 1.1f)
        {
            hasPassedThreshold = true;
            passedThresholdValue = value;
        }
    }

    shelfDelimiters.emplace_back((updateGlyphs.end() - 1)->second.rect.h);

    return std::make_pair(shelfDelimiters, slotDelimiters);
}

void UpdateDelimitersByDeltas(std::vector<std::pair<GlyphKey, Glyph>> &updateGlyphs,
                         std::vector<uint16> &shelfDelimiters,
                         std::vector<uint16> &slotDelimiters)
{
    auto maxW = GetMaxWidth(updateGlyphs);
    auto maxH = GetMaxHeight(updateGlyphs);
    if (*(shelfDelimiters.end() - 1) < maxH)
    {
        shelfDelimiters.emplace_back(static_cast<uint16>(maxH));
    }
    if (*(slotDelimiters.end() - 1) < maxW)
    {
        slotDelimiters.emplace_back(static_cast<uint16>(maxW));
    }
}


// todo triple check this section because it the it was tested how free slot was merged and leftover left.
bool MergeIntoIfPossible(uint16_2 &toMerge, std::vector<uint16_2> &intoContainer)
{
    uint16_2 *merged = nullptr;
    machine mergedIndex = 0;
    bool expandedRight = false;
    bool expandedLeft = false;
    for (int i = 0; i < intoContainer.size(); i++)
    {
        auto &into = intoContainer[i];
        assert(toMerge.x != into.y && toMerge.x - into.y != -1); // no intersection requirement
        if (toMerge.x - into.y == 1)
        {
            into.y = toMerge.y;
            assert(into.x <= into.y);
            merged = &into;
            mergedIndex = i;
            expandedRight = true;
            break;
        }
        assert(toMerge.y != into.x && into.x - toMerge.y != -1); // no intersection requirement
        if (into.x - toMerge.y == 1)
        {
            into.x = toMerge.x;
            assert(into.x <= into.y);
            merged = &into;
            mergedIndex = i;
            expandedLeft = true;
            break;
        }
    }

    if (merged)
    {
        if (expandedRight)
        {
            for (machine i = mergedIndex + 1; i < intoContainer.size(); i++)
            {
                auto &into = intoContainer[i];
                assert(into.x - merged->y != 0 && into.x - merged->y != -1); // no intersection requirement
                if (into.x - merged->y == 1)
                {
                    merged->y = into.y;
                    assert(merged->x <= merged->y);
                    intoContainer[i] = intoContainer[intoContainer.size() - 1]; // swap and erase last.
                    intoContainer.erase(intoContainer.end() - 1);
                    break;
                }
            }
        } else if (expandedLeft)
        {
            for (machine i = mergedIndex + 1; i < intoContainer.size(); i++)
            {
                auto &into = intoContainer[i];
                assert(merged->x - into.y != 0 && merged->x - into.y != -1); // no intersection requirement
                if (merged->x - into.y == 1)
                {
                    merged->x = into.x;
                    assert(merged->x <= merged->y);
                    intoContainer[i] = intoContainer[intoContainer.size() - 1];
                    intoContainer.erase(intoContainer.end() - 1);
                    break;
                }
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

void CheckContainerIntegrity(std::vector<uint16_2> &container, uint16 growTarget)
{
    std::sort(container.begin(), container.end());
    uint16_2 grow {0, 0};
    int index = 0;
    while (index < container.size())
    {
        if (grow.y == container[index].x)
        {
            grow.y = container[index].y + 1;
            container.erase(container.begin() + index);
            index = 0;
            continue;
        }
        index++;
    }

    assert(container.empty() && grow.y == growTarget);
}
