#ifndef _MATH_H_
#define _MATH_H_

#include <cmath>
#include <functional>
#include <random>

#define PI 3.1415926f

extern const float tolerance;

class Vec3
{
public:
	explicit Vec3() : data{0.0f, 0.0f, 0.0f} {}
	explicit Vec3(float x, float y, float z) : data{x, y, z} {}

	Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vec3 operator-() const { return Vec3(-x, -y, -z); }
	Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
	Vec3 operator*(float k) const { return Vec3(x * k, y * k, z * k); }
	friend Vec3 operator*(float k, const Vec3& v) { return v * k; }
	Vec3 operator*=(float k) { x *= k; y *= k; z *= k; return *this; }
	Vec3 operator/(float k) const { float k_inv = 1.0f / k; return Vec3(x * k_inv, y * k_inv, z * k_inv); }
	Vec3 operator/=(float k) { float k_inv = 1.0f / k; x *= k_inv; y *= k_inv; z *= k_inv; return *this; }
	Vec3 operator^(float k) const { return Vec3(std::pow(x, k), std::pow(y, k), std::pow(z, k)); }
	Vec3 operator^=(float k) { x = std::pow(x, k), y = std::pow(y, k), z = std::pow(z, k); return *this; }

	float Dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
	const Vec3& normalize() { *this *= (1.0f / std::sqrt(this->Dot(*this))); return *this; }
	Vec3 cross(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	float length() const { return std::sqrt(this->Dot(*this)); }

	float operator[](int i) const { return data[i]; }
	float& operator[](int i) { return data[i]; }

	union
	{
		float data[3];
		struct { float x, y, z; };
	};
};


class Point
{
public:
	explicit Point() : data{ 0.0f, 0.0f, 0.0f } {}
	explicit Point(float x, float y, float z) : data{x, y, z} {}

	Point operator+(const Vec3& v) const { return Point(x + v.x, y + v.y, z + v.z); }
	Point operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Point operator-(const Vec3& v) const { return Point(x - v.x, y - v.y, z - v.z); }
	Point operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vec3 operator-(const Point& p) const { return Vec3(x - p.x, y - p.y, z - p.z); }

	float operator[](int i) const { return data[i]; }
	float& operator[](int i) { return data[i]; }

	union
	{
		float data[3];
		struct { float x, y, z; };
	};
};

class Color
{
public:
	explicit Color() : data{ 0.0f,0.0f,0.0f } {}
	explicit Color(float r, float g, float b) : data{r, g, b} {}

	Color operator+(const Color& v) const { return Color(r + v.r, g + v.g, b + v.b); }
	Color operator+=(const Color& v) { r += v.r; g += v.g; b += v.b; return *this; }
	Color operator-(const Color& v) const { return Color(r - v.r, g - v.g, b - v.b); }
	Color operator-() const { return Color(-r, -g, -b); }
	Color operator*(const Color& v) const { return Color(r * v.r, g * v.g, b * v.b); }
	Color operator*(float k) const { return Color(r * k, g * k, b * k); }
	friend Color operator*(float k, const Color& c) { return c * k; }
	Color operator*=(float k) { r *= k; g *= k; b *= k; return *this; }
	Color operator/(float k) const { float k_inv = 1.0f / k; return Color(r * k_inv, g * k_inv, b * k_inv); }
	Color operator/=(float k) { float k_inv = 1.0f / k; r *= k_inv; g *= k_inv; b *= k_inv; return *this; }
	Color operator^(float k) const { return Color(std::pow(r, k), std::pow(g, k), std::pow(b, k)); }
	Color operator^=(float k) { r = std::pow(r, k), g = std::pow(g, k), b = std::pow(b, k); return *this; }

	float operator[](int i) const { return data[i]; }
	float& operator[](int i) { return data[i]; }

	union
	{
		float data[3];
		struct { float r, g, b; };
	};
};

class Martix
{
public:
	explicit Martix() {}
	explicit Martix(const Vec3& row1, const Vec3& row2, const Vec3& row3) : data{ row1,row2,row3 } {}

	Martix operator+(const Martix& m) const { return Martix(data[0] + m[0], data[1] + m[1], data[2] + m[2]); }
	Martix operator+=(const Martix& m) { data[0] += m[0]; data[1] += m[1]; data[2] += m[2]; return *this; }
	Martix operator-(const Martix& m) const { return Martix(data[0] - m[0], data[1] - m[1], data[2] - m[2]); }
	Martix operator-=(const Martix& m) { data[0] -= m[0]; data[1] -= m[1]; data[2] += m[2]; return *this; }
	Vec3 operator*(const Vec3& v) const { return Vec3(data[0].Dot(v), data[1].Dot(v), data[2].Dot(v)); }
	Point operator*(const Point& p) const { Vec3 v(p[0], p[1], p[2]); return Point(data[0].Dot(v), data[1].Dot(v), data[2].Dot(v)); }

	Vec3 operator[](int i) const { return data[i]; }
	Vec3& operator[](int i) { return data[i]; }

private:
	Vec3 data[3];
};

float random_float(float min = 0.0f, float max = 1.0f);
int random_int(int min, int max);
Vec3 random_sphere();
Vec3 random_hemisphere(const Vec3& v);
Vec3 random_unit_disk();
Color random_color(float min = 0.0f, float max = 1.0f);
Vec3 reflect(const Vec3& in, const Vec3& normal);
Vec3 refract(const Vec3& in, const Vec3& normal, float ir_ratio);
Martix rotate_y(float angle);

extern const Color White;
extern const Color Black;
extern const Color Red;
extern const Color Green;
extern const Color Blue;

#endif