#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "HitObject.h"
#include "../Render/Ray.h"

class Sphere : public HitObject
{
public:
	Sphere(Point center, float radius, const Color& color) : o(center), r(radius), HitObject(color) {}

	bool hit(const Ray& ray, HitRecord& record) override;

private:
	Point o;
	float r;
};


#endif