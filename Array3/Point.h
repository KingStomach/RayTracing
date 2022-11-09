#ifndef _POINT_H_
#define _POINT_H_

#include "Vec3.h"

class Point : public Array3
{
public:
	explicit Point(float x = 0.0, float y = 0.0, float z = 0.0) : Array3(x, y, z) {}
	Point(const Array3& arr) : Array3(arr) {}

	inline float x() { return data[0]; }
	inline float y() { return data[1]; }
	inline float z() { return data[2]; }

	inline Point operator+(const Vec3& v) const { return Array3::operator+(v); }
	inline Vec3 operator-(const Point& v) const { return Array3::operator-(v); }
};

#endif