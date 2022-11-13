#ifndef _HITOBJECT_H_
#define _HITOBJECT_H_

#include "Array3.h"
#include "Material.h"

struct HitRecord
{
	Point position;
	Vec3 normal;
	float t;
};

class Ray
{
public:
	Ray(const Point& point, const Vec3& dir) : p(point), d(dir) {}

	inline Point origin() const { return p; }
	inline Vec3 direction() const { return d; }
	inline Point at(float t) const { return p + d * t; }

private:
	Point p;
	Vec3 d;
};

class HitObject
{
public:
	virtual bool hit(const Ray& ray, HitRecord& record) const = 0;

};

class Sphere : public HitObject
{
public:
	Sphere(Point center, float radius) : o(center), r(radius) {}

	bool hit(const Ray& ray, HitRecord& record) const override;

private:
	Point o;
	float r;
};

#endif