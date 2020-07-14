#include "bmp.h"
using namespace std;
int main()
{
    try
    {
        {
            BMP bmp("2.1.bmp");
            cout << "水平像素数：" << bmp.getWidth() << '\n';
            cout << "垂直像素数：" << bmp.getHeight() << '\n';
            Color c;
            c = bmp.getPixel(0, 0);
            cout << "左下：\nR：" << static_cast<int>(c.R) << "\nG：" << static_cast<int>(c.G) << "\nB：" << static_cast<int>(c.B) << '\n';
            c = bmp.getPixel(bmp.getHeight() - 1, bmp.getWidth() - 1);
            cout << "右下：\nR：" << static_cast<int>(c.R) << "\nG：" << static_cast<int>(c.G) << "\nB：" << static_cast<int>(c.B) << '\n';
            bmp.conv2Gray();
            bmp.save("2.1.2.bmp");
        }
        {
            BMP bmp("2.2.bmp");
            Color c;
            for (unsigned int i = 0; i < bmp.getHeight(); i++)
            {
                for (unsigned int j = 0; j < bmp.getWidth(); j++)
                {
                    c = bmp.getPixel(i, j);
                    c.R = 255 - c.R;
                    c.G = 255 - c.G;
                    c.B = 255 - c.B;
                    bmp.setPixel(i, j, c);
                }
            }
            bmp.save("2.2.2.bmp");
        }
        return 0;
    }
    catch (const char *msg)
    {
        cerr << msg;
        return -1;
    }
}
