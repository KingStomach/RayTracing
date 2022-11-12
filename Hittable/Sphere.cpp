#include "Sphere.h"

bool Sphere::hit(const Ray& ray, HitRecord& record)
{
	Vec3 oc = ray.origin() - o;
	float a = ray.direction().Dot(ray.direction());
	float b = 2.0 * ray.direction().Dot(oc);
	float c = oc.Dot(oc) - r * r;
	float delta = b * b - 4.0 * a * c;
	if (delta < 0)
		return false;
	double t = std::sqrt(delta);
	double t1 = (-b - t) * 0.5f / a, t2 = (-b + t) * 0.5f / a;
	if (t2 < 0)
		return false;
	record.t = t1 > 0 ? t1 : t2;
	record.position = ray.at(record.t);
	record.normal = (record.position - o).normalize();
	return true;
}