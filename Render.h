#ifndef _RENDER_H_
#define _RENDER_H_

#include <array>
#include <string>
#include <vector>

#include "Math.h"

class Ray;

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
    inline std::array<std::uint8_t, 3> toUint8RGB(const Color& color) {
        return std::array<std::uint8_t, 3>{
            static_cast<std::uint8_t>(255.0 * clamp(color.b) + 0.5),
                static_cast<std::uint8_t>(255.0 * clamp(color.g) + 0.5),
                static_cast<std::uint8_t>(255.0 * clamp(color.r) + 0.5) };
    }
};

class Camera
{
public:
    Camera(const Point& position, const Point& lookat, const Vec3& up, float fov, float aspect_ratio);

    inline Point position() const { return p; }
    inline void setLen(float aperture, float focus_dist) { this->lens_radius = aperture / 2.0f; this->focus_dist = focus_dist; }

    Ray sample(float s, float v);

private:
    Point p;
    Point lookat;
    Vec3 up;
    float yoffset;
    float xoffset;
    float lens_radius;
    float focus_dist;
};

#endif