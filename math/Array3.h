#ifndef _ARRAY3_H_
#define _ARRAY3_H_

#include <assert.h>

class Array3
{
public:
	Array3(float x = 0.0, float y = 0.0, float z = 0.0) : data{ x,y,z } {}

	inline float at(int i) { assert(i < 3); return data[i]; }

protected:
	inline Array3 operator+(const Array3& arr) const { return Array3(data[0] + arr.data[0], data[1] + arr.data[1], data[2] + arr.data[2]); }
	inline Array3 operator-(const Array3& arr) const { return Array3(data[0] - arr.data[0], data[1] - arr.data[1], data[2] + arr.data[2]); }
	inline Array3 operator*(const Array3& arr) const { return Array3(data[0] * arr.data[0], data[1] * arr.data[1], data[2] * arr.data[2]); }
	inline Array3 operator*(float x) const { return Array3(data[0] * x, data[1] * x, data[2] * x); }

	float data[3];
};


#endif