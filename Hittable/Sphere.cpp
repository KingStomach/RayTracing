#include "Sphere.h"

bool Sphere::hit(const Ray& ray, HitRecord& record)
{
	Vec3 oc = ray.origin() - o;
	float a = ray.direction().Dot(ray.direction());
	float b = 2.0 * ray.direction().Dot(oc);
	float c = oc.Dot(oc) - r * r;
	float delta = b * b - 4.0 * a * c;
	if (delta < 0.0)
		return false;
	else
		return true;
}