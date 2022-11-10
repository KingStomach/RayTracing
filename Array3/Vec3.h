#ifndef _VEC3_H_
#define _VEC3_H_

#include "Array3.h"

#include <cmath>

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
	inline Vec3 operator*(float x) const { return Array3::operator*(x); }
	inline friend Vec3 operator*(float x, const Vec3& v) { return v * x; }
	inline float Dot(const Vec3& v) const { return data[0] * v.data[0] + data[1] * v.data[1] + data[2] * v.data[2]; }
	inline void normalize() { *this*= (1.0 / std::sqrt(this->Dot(*this))); }
	inline Vec3 cross(const Vec3& v) { return Vec3(data[1] * v.data[2] - data[2] * v.data[1], data[2] * v.data[0] - data[0] * v.data[2], data[0] * v.data[1] - data[1] * v.data[0]); }
};


#endif