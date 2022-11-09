#include "Bitmap.h"

#include <iostream>
#include <fstream>

void Bitmap::Fill(const Color& color)
{
    for (auto&& pixel : data)
        pixel = toUint8RGB(color);
}

bool Bitmap::SaveFile(const std::string& path) const
{
    std::ofstream out;
    out.open(path, std::ios::binary);
    if (!out.is_open())
    {
        std::cerr << "can't create file: " << path << "\n";
        out.close();
        return false;
    }

    BITMAPINFOHEADER info;
    uint32_t pixelsize = 3;
    uint32_t pitch = (width() * pixelsize + 3) & (~3);
    info.biSizeImage = pitch * height();
    uint32_t bfSize = 54 + info.biSizeImage;
    uint32_t zero = 0, offset = 54;
    info.biSize = 40;
    info.biWidth = width();
    info.biHeight = height();
    info.biPlanes = 1;
    info.biBitCount = pixelsize * 8;
    info.biCompression = 0;
    info.biXPelsPerMeter = 0xb12;
    info.biYPelsPerMeter = 0xb12;
    info.biClrUsed = 0;
    info.biClrImportant = 0;

    out.put(0x42);
    out.put(0x4d);
    out.write(reinterpret_cast<const char*>(&bfSize), 4);
    out.write(reinterpret_cast<const char*>(&zero), 4);
    out.write(reinterpret_cast<const char*>(&offset), 4);
    out.write(reinterpret_cast<const char*>(&info), sizeof(info));
    if (!out.is_open())
    {
        std::cerr << "can't write header: " << path << "\n";
        out.close();
        return false;
    }
    std::uint8_t B, G, R;
    for (int y = 0; y < height(); y++)
    {
        uint32_t padding = pitch - width() * pixelsize;
        for (int x = 0; x < width(); x++)
        {
            out.write(reinterpret_cast<const char*>(&data[getIndex(x, y)]), pixelsize);
            if (!out.is_open())
            {
                std::cerr << "can't write pixel " << std::to_string(x) << " " << std::to_string(y) << "\n";
                out.close();
                return false;
            }
        }
        for (int i = 0; i < (int)padding; i++) out.put(0);
    }

    out.close();
    return true;
}