#include "HitObject.h"

bool Scene::hit(const Ray& ray, float min, float max, HitRecord& record) const
{
	float closest = max;
	bool ishit = false;
	for (auto &&o : v)
	{
		if (o->hit(ray, min, closest, record))
		{
			ishit = true;
			closest = record.t;
			record.object = o;
		}
	}
	return ishit;
}

bool Sphere::hit(const Ray& ray, float min, float max, HitRecord& record) const
{
	Vec3 oc = ray.origin() - o;
	float a = ray.direction().Dot(ray.direction());
	float b = 2.0 * ray.direction().Dot(oc);
	float c = oc.Dot(oc) - r * r;
	float delta = b * b - 4.0 * a * c;

	if (delta < 0)
		return false;
	double t = std::sqrt(delta);
	double t1 = (-b - t) * 0.5f / a;
	if (t1 < min || t1 > max)
		return false;

	record.t = t1;
	record.position = ray.at(t1);
	record.normal = (record.position - o).normalize();
	return true;
}

