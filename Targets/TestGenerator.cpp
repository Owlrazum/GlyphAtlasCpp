#include "PrimitiveTypes.h"
#include "FileUtils.h"

#include <vector>
#include <fstream>

void GenerateTestCase(
        machine testNumber,
        machine passCount,
        machine fontCount,
        machine keysCountInPass);

int main()
{
    GenerateTestCase(0, 5, 2, 100);
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
        WriteGlyphTestData(out, fontCount, keysCountInPass);
        if (i != passCount - 1)
        {
            out << "\n";
        }
    }
}