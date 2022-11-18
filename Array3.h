#ifndef _ARRAY3_H_
#define _ARRAY3_H_

#include<cmath>
#include <functional>
#include <random>

#define PI 3.1415926

inline float random_float()
{
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	static std::function<float()> rand_generator =
		std::bind(distribution, generator);
	return rand_generator();
}

class Array3
{
protected:
	explicit Array3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : data{ x,y,z } {}

	inline Array3 operator+(const Array3& arr) const { return Array3(data[0] + arr.data[0], data[1] + arr.data[1], data[2] + arr.data[2]); }
	inline Array3 operator+=(const Array3& arr) { data[0] += arr.data[0]; data[1] += arr.data[1]; data[2] += arr.data[2]; return *this; }
	inline Array3 operator-(const Array3& arr) const { return Array3(data[0] - arr.data[0], data[1] - arr.data[1], data[2] - arr.data[2]); }
	inline Array3 operator-() const { return Array3(-data[0], -data[1], -data[2]); }
	inline Array3 operator*(const Array3& arr) const { return Array3(data[0] * arr.data[0], data[1] * arr.data[1], data[2] * arr.data[2]); }
	inline Array3 operator*(float x) const { return Array3(data[0] * x, data[1] * x, data[2] * x); }
	inline Array3 operator*=(float x) { data[0] *= x; data[1] *= x; data[2] *= x; return *this; }
	inline Array3 operator/(float x) const { return Array3(data[0] / x, data[1] / x, data[2] / x); }
	inline Array3 operator/=(float x) { data[0] /= x; data[1] /= x; data[2] /= x; return *this; }
	inline Array3 operator^(float x) const { return Array3(std::pow(data[0], x), std::pow(data[1], x), std::pow(data[2], x)); }
	inline Array3 operator^=(float x) { data[0] = std::pow(data[0], x), data[1] = std::pow(data[1], x), data[2] = std::pow(data[2], x); return *this; }

	float data[3];
};

class Vec3 : public Array3
{
public:
	explicit Vec3(float x = 0.0, float y = 0.0, float z = 0.0) : Array3(x, y, z) {}
	Vec3(const Array3& arr) : Array3(arr) {}

	inline float x() { return data[0]; }
	inline float y() { return data[1]; }
	inline float z() { return data[2]; }

	inline Vec3 operator+(const Vec3& v) const { return Array3::operator+(v); }
	inline Vec3 operator-(const Vec3& v) const { return Array3::operator-(v); }
	inline Vec3 operator-() const { return Array3::operator-(); }
	inline Vec3 operator*(float x) const { return Array3::operator*(x); }
	inline friend Vec3 operator*(float x, const Vec3& v) { return v * x; }
	inline float Dot(const Vec3& v) const { return data[0] * v.data[0] + data[1] * v.data[1] + data[2] * v.data[2]; }
	inline const Vec3& normalize() { *this *= (1.0f / std::sqrt(this->Dot(*this))); return *this; }
	inline Vec3 cross(const Vec3& v) const { return Vec3(data[1] * v.data[2] - data[2] * v.data[1], data[2] * v.data[0] - data[0] * v.data[2], data[0] * v.data[1] - data[1] * v.data[0]); }
	inline float length() const { return std::sqrt(this->Dot(*this)); }

	inline static Vec3 random_sphere()
	{
		float theta = random_float() * PI, phi = random_float() * PI * 2.0f;
		return Vec3(std::cos(theta) * std::cos(phi), std::cos(theta) * std::sin(phi), std::sin(theta));
	}
	inline static Vec3 random_hemisphere(const Vec3& v)
	{	
		Vec3 res = random_sphere();
		if (res.Dot(v) > 0.0)
			return res;
		else
			return -res;
	}
	inline static Vec3 reflect(const Vec3& in, const Vec3& normal) { return in - 2.0f * in.Dot(normal) * normal; }
	inline static Vec3 refract(const Vec3& in, const Vec3& normal, float ir_ratio)
	{
		float cos_theta1 = -in.Dot(normal), sin_theta1 = std::sqrt(1.0f - cos_theta1 * cos_theta1);
		float sin_theta2 = ir_ratio * sin_theta1, cos_theta2 = std::sqrt(1.0f - sin_theta2 * sin_theta2);
		Vec3 vertical = -cos_theta2 * normal;
		Vec3 horizontal = sin_theta2 * (in + cos_theta1 * normal).normalize();
		return vertical + horizontal;

		//Vec3 vertical = ir_ratio * (in + std::min(-in.Dot(normal), 1.0f) * normal);
		//Vec3 horizontal = -std::sqrt(1.0f - vertical.length()) * normal;
		//return vertical + horizontal;
	}
};

class Point : public Array3
{
public:
	explicit Point(float x = 0.0f, float y = 0.0f, float z = 0.0f) : Array3(x, y, z) {}
	Point(const Array3& arr) : Array3(arr) {}

	inline float x() { return data[0]; }
	inline float y() { return data[1]; }
	inline float z() { return data[2]; }

	inline Point operator+(const Vec3& v) const { return Array3::operator+(v); }
	inline Vec3 operator-(const Point& v) const { return Array3::operator-(v); }
};

class Color : public Array3
{
public:
	explicit Color(float x = 0.0f, float y = 0.0f, float z = 0.0f) : Array3(x, y, z) {}
	Color(const Array3& arr) : Array3(arr) {}

	inline float r() const { return data[0]; }
	inline float g() const { return data[1]; }
	inline float b() const { return data[2]; }

	inline Color operator+(const Color& v) const { return Array3::operator+(v); }
	inline Color operator+=(const Color& v) { return Array3::operator+=(v); }
	inline Color operator-(const Color& v) const { return Array3::operator-(v); }
	inline Color operator*(const Color& v) const { return Array3::operator*(v); }
	inline Color operator*(float x) const { return Array3::operator*(x); }
	inline friend Color operator*(float x, const Color& v) { return v * x; }
	inline Color operator/(float x) const { return Array3::operator/(x); }
	inline Color operator/=(float x) { return Array3::operator/=(x); }
	inline Color operator^(float x) const { return Array3::operator^(x); }
	inline Color operator^=(float x) { return Array3::operator^=(x); }
};

const Color White(1.0f, 1.0f, 1.0f);
const Color Black(0.0f, 0.0f, 0.0f);
const Color Red(1.0f, 0.0f, 0.0f);
const Color Green(0.0f, 1.0f, 0.0f);
const Color Blue(0.0f, 0.0f, 1.0f);

#endif