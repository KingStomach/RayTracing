#ifndef _RAY_H_
#define _RAY_H_

#include "../Array3/Point.h"

class Ray
{
public:
	Ray(const Point& point, const Vec3& dir) : p(point), d(dir) {}

	inline Point origin() const { return p; }
	inline Vec3 direction() const { return d; }
	inline Point at(float t) { return p + d * t; }

private:
	Point p;
	Vec3 d;
};


#endif