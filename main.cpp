#include "bmp.h"
#include <iostream>
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
            bmp.reverseImageColor();
            bmp.save("2.2.2.bmp");
        }
        {
            BMP bmp("2.3.bmp");
            Color c;
            bmp.equalizeRedBlue();
            bmp.save("2.3.2.bmp");
        }
        {
            BMP bmp("3.3.bmp");
            bmp.pixelize(32, 32, 128, 128, 8);
            bmp.save("3.3.1.bmp");
        }
        return 0;
    }
    catch (const char *msg)
    {
        cerr << msg;
        return -1;
    }
}
