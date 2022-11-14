#include "Material.h"

bool Diffuse::scatter(const Ray& in, const HitRecord& record, Color& attenuation, Ray& out) const
{
	out = Ray(record.position, Vec3::random_hemisphere(record.normal));
	attenuation = albedo;
	return true;
}