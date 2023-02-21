#include "EndPoints.h"

bool MergeIntoIfPossible(Pair &toMerge, std::vector<Pair> &intoContainer)
{
    int mergedIndex = -1;
    bool isByX = false;
    for (int i = 0; i < intoContainer.size(); i++)
    {
        if (abs(toMerge.x - intoContainer[i].y) == 1)
        {
            intoContainer[i].y = toMerge.y;
            mergedIndex = i;
            isByX = true;
            break;
        }
        if (abs(toMerge.y - intoContainer[i].x) == 1)
        {
            intoContainer[i].x = toMerge.x;
            mergedIndex = i;
            break;
        }
    }

    if (mergedIndex >= 0)
    {
        if (isByX)
        {
            for (int i = mergedIndex + 1; i < intoContainer.size(); i++)
            {
                if (abs(intoContainer[mergedIndex].y - intoContainer[i].x) == 1)
                {
                    intoContainer[mergedIndex].y = intoContainer[i].y;
                    intoContainer.begin() + i = intoContainer.end() - 1; // swap and erase last
                    intoContainer.erase(intoContainer.end() - 1);
                    return true;
                }
            }
        } else
        {
            for (int i = mergedIndex + 1; i < intoContainer.size(); i++)
            {
                if (abs(intoContainer[mergedIndex].x - intoContainer[i].y) == 1)
                {
                    intoContainer[mergedIndex].x = intoContainer[i].x;
                    intoContainer.begin() + i = intoContainer.end() - 1;
                    intoContainer.erase(intoContainer.end() - 1);
                    return true;
                }
            }
        }
        return true;
    }
    return false;
}
