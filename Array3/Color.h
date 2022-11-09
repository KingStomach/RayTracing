#ifndef _COLOR_H_
#define _COLOR_H_

#include "Array3.h"

class Color : public Array3
{
public:
	explicit Color(float x = 0.0, float y = 0.0, float z = 0.0) : Array3(x, y, z) {}
	Color(const Array3& arr) : Array3(arr) {}

	inline float r() const { return data[0]; }
	inline float g() const { return data[1]; }
	inline float b() const { return data[2]; }

	inline Color operator+(const Color& v) const { return Array3::operator+(v); }
	inline Color operator-(const Color& v) const { return Array3::operator-(v); }
	inline Color operator*(const Color& v) const { return Array3::operator*(v); }
};

const Color White(1.0, 1.0, 1.0);
const Color Black(0.0, 0.0, 0.0);
const Color Red(1.0, 0.0, 0.0);
const Color Green(0.0, 1.0, 0.0);
const Color Blue(0.0, 0.0, 1.0);

#endif