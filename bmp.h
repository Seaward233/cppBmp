#ifndef BMP_
#define BMP_
#include <fstream>
#include <string>
namespace std
{
    class Color
    {
    public:
        uint8_t B;
        uint8_t G;
        uint8_t R;
        Color();
        Color(uint8_t, uint8_t, uint8_t);
    };
    class BitmapFileHeader
    {
    public:
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    };
    class BitmapInfoHeader
    {
    public:
        uint32_t biSize;
        uint32_t biWidth;
        uint32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        uint32_t biXPelsPerMeter;
        uint32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };
    class RGBQuad
    {
    public:
        uint8_t rgbBlue;
        uint8_t rgbGreen;
        uint8_t rgbRed;
        uint8_t rgbReserved;
    };
    class BitmapInfo
    {
    public:
        uint32_t width;
        uint32_t height;
        uint16_t bitCount;
        uint32_t zeronumperline;
        Color **image;
        uint8_t **grayImage;
        BitmapInfo(uint32_t, uint32_t, uint16_t);
        ~BitmapInfo();
    };
    class BMP
    {
    private:
        BitmapFileHeader bmpFileHeader;
        BitmapInfoHeader bmpInfoHeader;
        RGBQuad *rgbQuad;
        BitmapInfo *bmpInfo;

    public:
        BMP(string); //构造函数，用于把path路径下的bmp图像文件的文件头、信息头、调色板、颜色信息读入私有属性中
        ~BMP();
        Color getPixel(uint32_t, uint32_t);          //获取第i行、第j列的颜色信息（彩色），行号以最下一行为第0行，列号以最左一列为第0列
        uint8_t getPixelG(uint32_t, uint32_t);       //获取第i行、第j列的颜色信息（灰度），行号以最下一行为第0行，列号以最左一列为第0列
        void setPixel(uint32_t, uint32_t, Color);    //设置第i行、第j列的颜色（彩色），行号以最下一行为第0行，列号以最左一列为第0列
        void setPixelG(uint32_t, uint32_t, uint8_t); //设置第i行、第j列的颜色（灰度），行号以最下一行为第0行，列号以最左一列为第0列
        uint32_t getHeight();
        uint32_t getWidth();
        void reverseImageColor();
        void equalizeRedBlue();
        Color **getImage();
        void conv2Gray();  //把彩色图片转成灰度
        void save(string); //把图像保存成文件
        void pixelize(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
    };
} // namespace std
#endif // BMP_