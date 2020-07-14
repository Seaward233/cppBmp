#include "bmp.h"
namespace std
{
    Color::Color() {}
    Color::Color(uint8_t R, uint8_t G, uint8_t B) : R(R), G(G), B(B) {}
    BitmapInfo::BitmapInfo(uint32_t width, uint32_t height, uint16_t bitCount) : width(width), height(height), bitCount(bitCount)
    {
        if (bitCount == 24)
        {
            image = new Color *[height];
            for (uint32_t i = 0; i < height; i++)
            {
                image[i] = new Color[width];
            }
            zeronumperline = (uint32_t)(4 - (width * bitCount / 8) % 4) % 4;
            grayImage = nullptr;
        }
        else if (bitCount == 8)
        {
            grayImage = new uint8_t *[height];
            for (uint32_t i = 0; i < height; i++)
            {
                grayImage[i] = new uint8_t[width];
            }
            zeronumperline = (uint32_t)(4 - (width * bitCount / 8) % 4) % 4;
            image = nullptr;
        }
        else
        {
            throw "不支持的颜色深度";
        }
    }
    BitmapInfo::~BitmapInfo()
    {
        if (image != nullptr)
        {
            for (uint32_t i = 0; i < height; i++)
            {
                if (image[i] != nullptr)
                    delete[] image[i];
            }
            delete[] image;
        }
        if (grayImage != nullptr)
        {
            for (uint32_t i = 0; i < height; i++)
            {
                if (grayImage[i] != nullptr)
                    delete[] grayImage[i];
            }
            delete[] grayImage;
        }
    }
    BMP::BMP(string path)
    {
        ifstream fin(path, ios::binary);
        if (!fin)
        {
            throw "文件打开失败";
        }
        //读取文件头
        fin.read((char *)&bmpFileHeader.bfType, sizeof(bmpFileHeader.bfType));
        fin.read((char *)&bmpFileHeader.bfSize, sizeof(bmpFileHeader.bfSize));
        fin.read((char *)&bmpFileHeader.bfReserved1, sizeof(bmpFileHeader.bfReserved1));
        fin.read((char *)&bmpFileHeader.bfReserved2, sizeof(bmpFileHeader.bfReserved2));
        fin.read((char *)&bmpFileHeader.bfOffBits, sizeof(bmpFileHeader.bfOffBits));
        if (bmpFileHeader.bfType != 0x4D42)
        {
            throw "不是有效的BMP格式图片";
        }
        //读取信息头
        fin.read((char *)&bmpInfoHeader.biSize, sizeof(bmpInfoHeader.biSize));
        fin.read((char *)&bmpInfoHeader.biWidth, sizeof(bmpInfoHeader.biWidth));
        fin.read((char *)&bmpInfoHeader.biHeight, sizeof(bmpInfoHeader.biHeight));
        fin.read((char *)&bmpInfoHeader.biPlanes, sizeof(bmpInfoHeader.biPlanes));
        fin.read((char *)&bmpInfoHeader.biBitCount, sizeof(bmpInfoHeader.biBitCount));
        fin.read((char *)&bmpInfoHeader.biCompression, sizeof(bmpInfoHeader.biCompression));
        fin.read((char *)&bmpInfoHeader.biSizeImage, sizeof(bmpInfoHeader.biSizeImage));
        fin.read((char *)&bmpInfoHeader.biXPelsPerMeter, sizeof(bmpInfoHeader.biXPelsPerMeter));
        fin.read((char *)&bmpInfoHeader.biYPelsPerMeter, sizeof(bmpInfoHeader.biYPelsPerMeter));
        fin.read((char *)&bmpInfoHeader.biClrUsed, sizeof(bmpInfoHeader.biClrUsed));
        fin.read((char *)&bmpInfoHeader.biClrImportant, sizeof(bmpInfoHeader.biClrImportant));
        if (bmpInfoHeader.biBitCount != 8 && bmpInfoHeader.biBitCount != 24)
        {
            throw "不支持的颜色深度";
        }
        //如果是灰度图像，读取调色板
        if (bmpInfoHeader.biBitCount == 8)
        {
            rgbQuad = new RGBQuad[256];
            for (int i = 0; i < 256; i++)
            {
                fin.read((char *)&rgbQuad[i].rgbBlue, sizeof(rgbQuad[i].rgbBlue));
                fin.read((char *)&rgbQuad[i].rgbGreen, sizeof(rgbQuad[i].rgbGreen));
                fin.read((char *)&rgbQuad[i].rgbRed, sizeof(rgbQuad[i].rgbRed));
                fin.read((char *)&rgbQuad[i].rgbReserved, sizeof(rgbQuad[i].rgbReserved));
            }
        }
        else
        {
            rgbQuad = nullptr;
        }
        //读取颜色信息
        bmpInfo = new BitmapInfo(bmpInfoHeader.biWidth, bmpInfoHeader.biHeight, bmpInfoHeader.biBitCount);
        if (bmpInfoHeader.biBitCount == 24)
        {
            for (uint32_t i = 0; i < bmpInfoHeader.biHeight; i++)
            {
                for (uint32_t j = 0; j < bmpInfoHeader.biWidth; j++)
                {
                    fin.read((char *)&bmpInfo->image[i][j].B, sizeof(bmpInfo->image[i][j].B));
                    fin.read((char *)&bmpInfo->image[i][j].G, sizeof(bmpInfo->image[i][j].G));
                    fin.read((char *)&bmpInfo->image[i][j].R, sizeof(bmpInfo->image[i][j].R));
                }
                fin.seekg(bmpInfo->zeronumperline, ios::cur);
            }
        }
        else if (bmpInfoHeader.biBitCount == 8)
        {
            for (uint32_t i = 0; i < bmpInfoHeader.biHeight; i++)
            {
                for (uint32_t j = 0; j < bmpInfoHeader.biWidth; j++)
                {
                    fin.read((char *)&bmpInfo->grayImage[i][j], sizeof(bmpInfo->grayImage[i][j]));
                }
                fin.seekg(bmpInfo->zeronumperline, ios::cur);
            }
        }
        else
        {
            throw "不支持的颜色深度";
        }
        fin.close();
    }
    BMP::~BMP()
    {
        if (rgbQuad != nullptr)
        {
            delete[] rgbQuad;
        }
        delete bmpInfo;
    }
    Color BMP::getPixel(uint32_t i, uint32_t j)
    {
        if (i < 0 || i >= bmpInfoHeader.biHeight || j < 0 || j >= bmpInfoHeader.biWidth)
        {
            throw "坐标超出图像范围";
        }
        return bmpInfo->image[i][j];
    }
    uint8_t BMP::getPixelG(uint32_t i, uint32_t j)
    {
        if (i < 0 || i >= bmpInfoHeader.biHeight || j < 0 || j >= bmpInfoHeader.biWidth)
        {
            throw "坐标超出图像范围";
        }
        return bmpInfo->grayImage[i][j];
    }
    void BMP::setPixel(uint32_t i, uint32_t j, Color c)
    {
        if (i < 0 || i >= bmpInfoHeader.biHeight || j < 0 || j >= bmpInfoHeader.biWidth)
        {
            throw "坐标超出图像范围";
        }
        bmpInfo->image[i][j] = c;
    }
    void BMP::setPixelG(uint32_t i, uint32_t j, uint8_t c)
    {
        if (i < 0 || i >= bmpInfoHeader.biHeight || j < 0 || j >= bmpInfoHeader.biWidth)
        {
            throw "坐标超出图像范围";
        }
        bmpInfo->grayImage[i][j] = c;
    }
    uint32_t BMP::getHeight()
    {
        return bmpInfoHeader.biHeight;
    }
    uint32_t BMP::getWidth()
    {
        return BMP::bmpInfoHeader.biWidth;
    }
    void BMP::conv2Gray()
    {
        rgbQuad = new RGBQuad[256];
        for (int i = 0; i < 256; i++)
        {
            rgbQuad[i].rgbBlue = i;
            rgbQuad[i].rgbGreen = i;
            rgbQuad[i].rgbRed = i;
            rgbQuad[i].rgbReserved = 0;
        }
        bmpInfoHeader.biBitCount = 8;
        bmpFileHeader.bfOffBits = 14 + 40 + 1024;
        bmpInfo->zeronumperline = (uint32_t)(4 - (bmpInfoHeader.biWidth * bmpInfoHeader.biBitCount / 8) % 4) % 4;
        bmpInfoHeader.biSizeImage = bmpInfoHeader.biHeight * (bmpInfoHeader.biWidth + bmpInfo->zeronumperline);
        bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmpInfoHeader.biSizeImage;
        bmpInfo->grayImage = new uint8_t *[bmpInfoHeader.biHeight];
        for (uint32_t i = 0; i < bmpInfoHeader.biHeight; i++)
        {
            bmpInfo->grayImage[i] = new uint8_t[bmpInfoHeader.biWidth];
        }
        for (uint32_t i = 0; i < bmpInfoHeader.biHeight; i++)
        {
            for (uint32_t j = 0; j < bmpInfoHeader.biWidth; j++)
            {
                bmpInfo->grayImage[i][j] = (uint8_t)(bmpInfo->image[i][j].R * 0.299 + bmpInfo->image[i][j].G * 0.587 + bmpInfo->image[i][j].B * 0.114);
            }
        }
        if (bmpInfo->image != nullptr)
        {
            for (uint32_t i = 0; i < bmpInfoHeader.biHeight; i++)
            {
                if (bmpInfo->image[i] != nullptr)
                    delete[] bmpInfo->image[i];
            }
            delete[] bmpInfo->image;
            bmpInfo->image = nullptr;
        }
    }
    void BMP::save(string path)
    {
        ofstream fout(path, ios::binary);
        if (!fout)
        {
            throw "文件打开失败";
        }
        //写文件头
        fout.write((char *)&bmpFileHeader.bfType, sizeof(bmpFileHeader.bfType));
        fout.write((char *)&bmpFileHeader.bfSize, sizeof(bmpFileHeader.bfSize));
        fout.write((char *)&bmpFileHeader.bfReserved1, sizeof(bmpFileHeader.bfReserved1));
        fout.write((char *)&bmpFileHeader.bfReserved2, sizeof(bmpFileHeader.bfReserved2));
        fout.write((char *)&bmpFileHeader.bfOffBits, sizeof(bmpFileHeader.bfOffBits));
        if (bmpFileHeader.bfType != 0x4D42)
        {
            throw "不是有效的BMP格式图片";
        }
        //写信息头
        fout.write((char *)&bmpInfoHeader.biSize, sizeof(bmpInfoHeader.biSize));
        fout.write((char *)&bmpInfoHeader.biWidth, sizeof(bmpInfoHeader.biWidth));
        fout.write((char *)&bmpInfoHeader.biHeight, sizeof(bmpInfoHeader.biHeight));
        fout.write((char *)&bmpInfoHeader.biPlanes, sizeof(bmpInfoHeader.biPlanes));
        fout.write((char *)&bmpInfoHeader.biBitCount, sizeof(bmpInfoHeader.biBitCount));
        fout.write((char *)&bmpInfoHeader.biCompression, sizeof(bmpInfoHeader.biCompression));
        fout.write((char *)&bmpInfoHeader.biSizeImage, sizeof(bmpInfoHeader.biSizeImage));
        fout.write((char *)&bmpInfoHeader.biXPelsPerMeter, sizeof(bmpInfoHeader.biXPelsPerMeter));
        fout.write((char *)&bmpInfoHeader.biYPelsPerMeter, sizeof(bmpInfoHeader.biYPelsPerMeter));
        fout.write((char *)&bmpInfoHeader.biClrUsed, sizeof(bmpInfoHeader.biClrUsed));
        fout.write((char *)&bmpInfoHeader.biClrImportant, sizeof(bmpInfoHeader.biClrImportant));
        if (bmpInfoHeader.biBitCount != 8 && bmpInfoHeader.biBitCount != 24)
        {
            throw "不支持的颜色深度";
        }
        //如果是灰度图像，写调色板
        if (bmpInfoHeader.biBitCount == 8)
        {
            for (int i = 0; i < 256; i++)
            {
                fout.write((char *)&rgbQuad[i].rgbBlue, sizeof(rgbQuad[i].rgbBlue));
                fout.write((char *)&rgbQuad[i].rgbGreen, sizeof(rgbQuad[i].rgbGreen));
                fout.write((char *)&rgbQuad[i].rgbRed, sizeof(rgbQuad[i].rgbRed));
                fout.write((char *)&rgbQuad[i].rgbReserved, sizeof(rgbQuad[i].rgbReserved));
            }
        }
        //写颜色信息
        if (bmpInfoHeader.biBitCount == 24)
        {
            for (uint32_t i = 0; i < bmpInfoHeader.biHeight; i++)
            {
                for (uint32_t j = 0; j < bmpInfoHeader.biWidth; j++)
                {
                    fout.write((char *)&bmpInfo->image[i][j].B, sizeof(bmpInfo->image[i][j].B));
                    fout.write((char *)&bmpInfo->image[i][j].G, sizeof(bmpInfo->image[i][j].G));
                    fout.write((char *)&bmpInfo->image[i][j].R, sizeof(bmpInfo->image[i][j].R));
                }
                for (uint32_t j = 0; j < bmpInfo->zeronumperline; j++)
                {
                    fout.put(0);
                }
            }
        }
        else if (bmpInfoHeader.biBitCount == 8)
        {
            for (uint32_t i = 0; i < bmpInfoHeader.biHeight; i++)
            {
                for (uint32_t j = 0; j < bmpInfoHeader.biWidth; j++)
                {
                    fout.write((char *)&bmpInfo->grayImage[i][j], sizeof(bmpInfo->grayImage[i][j]));
                }
                for (uint32_t j = 0; j < bmpInfo->zeronumperline; j++)
                {
                    fout.put(0);
                }
            }
        }
        else
        {
            throw "不支持的颜色深度";
        }
        fout.close();
    }
} // namespace std