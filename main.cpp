#include "bmp.h"
using namespace std;
int main()
{
    BMP bmp("1.bmp");
    Color c;
    for (unsigned int i = 0; i < bmp.getHeight(); i++)
    {
        for (unsigned int j = 0; j < bmp.getWidth(); j++)
        {
            c = bmp.getPixel(i, j);
            c.R *= 2;
            c.G *= 2;
            c.B *= 2;
            bmp.setPixel(i, j, c);
        }
    }
    bmp.save("1.2.bmp");
    return 0;
}
