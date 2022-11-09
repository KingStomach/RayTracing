#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <array>
#include <string>
#include <vector>

#include "../Array3/Color.h"

class Bitmap
{
public:
    explicit Bitmap(const int width = 0, const int height = 0) : w(width), h(height) { data.resize(width * height); }

    int width() const { return w; }
    int height() const { return h; }

    void Fill(const Color& color);
    bool SaveFile(const std::string& path) const;
    inline void SetPixel(int x, int y, float r, float g, float b) { this->SetPixel(x, y, Color(r, g, b)); }
    inline void SetPixel(int x, int y, const Color& color) { data[getIndex(x, y)] = toUint8RGB(color); }

private:
    int w;
    int h;
    std::vector<std::array<std::uint8_t, 3>> data;

    struct BITMAPINFOHEADER
    {
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
    inline float clamp(float x) const { return std::min(std::max(x, 0.0f), 1.0f); }
    inline std::array<std::uint8_t, 3> toUint8RGB(const Color& color) { return std::array<std::uint8_t, 3>{
        static_cast<std::uint8_t>(255.0 * clamp(color.b()) + 0.5),
        static_cast<std::uint8_t>(255.0 * clamp(color.g()) + 0.5),
        static_cast<std::uint8_t>(255.0 * clamp(color.r()) + 0.5) }; }
};

#endif