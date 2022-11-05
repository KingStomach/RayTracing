#ifndef _COLOR_H_
#define _COLOR_H_

class Color : public Array3
{
public:
	Color(float x = 0.0, float y = 0.0, float z = 0.0) : Array3(x, y, z) {}
	Color(const Array3& arr) : Array3(arr) {}

	inline float r() { return data[0]; }
	inline float g() { return data[1]; }
	inline float b() { return data[2]; }

	inline Color operator+(const Color& v) const { return Array3::operator+(v); }
	inline Color operator-(const Color& v) const { return Array3::operator-(v); }
	inline Color operator*(const Color& v) const { return Array3::operator(v); }
};

#endif