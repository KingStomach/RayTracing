#ifndef _ARRAY3_H_
#define _ARRAY3_H_

class Array3
{
public:
	explicit Array3(float x = 0.0, float y = 0.0, float z = 0.0) : data{ x,y,z } {}

protected:
	inline Array3 operator+(const Array3& arr) const { return Array3(data[0] + arr.data[0], data[1] + arr.data[1], data[2] + arr.data[2]); }
	inline Array3 operator-(const Array3& arr) const { return Array3(data[0] - arr.data[0], data[1] - arr.data[1], data[2] + arr.data[2]); }
	inline Array3 operator*(const Array3& arr) const { return Array3(data[0] * arr.data[0], data[1] * arr.data[1], data[2] * arr.data[2]); }
	inline Array3 operator*(float x) const { return Array3(data[0] * x, data[1] * x, data[2] * x); }
	inline Array3 operator*=(float x) { data[0] *= x; data[1] *= x; data[2] *= x; return *this; }

	float data[3];
};


#endif