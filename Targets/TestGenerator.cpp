#include "PrimitiveTypes.h"
#include "FileUtils.h"

#include <vector>
#include <fstream>

// Generates files in TestData/ using the following conventions:
// 1. font_x.txt mimics font by listing all rects one by one on each line
// 2. test.x.txt lists lines each containing font index leading to font_x.txt file,
// then a list of glyph indices inside that font leading to Rect
void GenerateTestCase(
        machine testNumber,
        machine passCount,
        machine fontCount,
        machine keysCountInPass);

int main()
{
    GenerateTestCase(0, 20, 5, 400);
    return 0;
}

void GenerateTestCase(
        machine testNumber,
        machine passCount,
        machine fontCount,
        machine keysCountInPass)
{
    std::ofstream out {GetTestGlyphKeysPath(testNumber)};
    for (machine i = 0; i < passCount; i++)
    {
        WriteTestGlyphKeys(out, fontCount, keysCountInPass);
        if (i != passCount - 1)
        {
            out << "\n";
        }
    }
}