#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <string>
#include <vector>

class BitmapColor
{
public:
    BitmapColor(std::uint8_t r = 255, std::uint8_t g = 255, std::uint8_t b = 255) : R(r), B(b), G(g) {}
    BitmapColor(float r, float g, float b) : R(static_cast<uint8_t>(255.0 * clamp(r) + 0.5)), G(static_cast<uint8_t>(255.0 * clamp(g) + 0.5)), B(static_cast<uint8_t>(255.0 * clamp(b) + 0.5)) {};

private:
    std::uint8_t B;
    std::uint8_t G;
    std::uint8_t R;

    inline float clamp(float x)
    { return std::min(std::max(x, 0.0f), 1.0f); }
};

class Bitmap
{
public:
    Bitmap(const int width = 0, const int height = 0) : w(width), h(height) { data.resize(width * height); }

    int width() const { return w; }
    int height() const { return h; }

    void Fill(const BitmapColor& color);
    bool SaveFile(const std::string& path) const;
    inline void SetPixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b) { data[getIndex(x, y)] = BitmapColor(r, g, b); }
    inline void SetPixel(int x, int y, const BitmapColor& color) { data[getIndex(x, y)] = color; }

private:
    int w;
    int h;
    std::vector<BitmapColor> data;

    struct BITMAPINFOHEADER { // bmih  
        uint32_t	biSize;
        uint32_t	biWidth;
        int32_t		biHeight;
        uint16_t	biPlanes;
        uint16_t	biBitCount;
        uint32_t	biCompression;
        uint32_t	biSizeImage;
        uint32_t	biXPelsPerMeter;
        uint32_t	biYPelsPerMeter;
        uint32_t	biClrUsed;
        uint32_t	biClrImportant;
    };

    inline int getIndex(int x, int y) const { return x * h + y; }
};

#endif