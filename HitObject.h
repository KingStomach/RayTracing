#ifndef _HITOBJECT_H_
#define _HITOBJECT_H_

#include "Array3.h"

class Material;

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

	std::shared_ptr<Material> material() const { return m; }

protected:
	HitObject(const std::shared_ptr<Material>& material) : m(material) {}

private:
	std::shared_ptr<Material> m;
};

class Sphere : public HitObject
{
public:
	Sphere(const Point& center, float radius, const std::shared_ptr<Material>& material) : o(center), r(radius), HitObject(material) {}

	bool hit(const Ray& ray, HitRecord& record) const override;

private:
	Point o;
	float r;
};

#endif