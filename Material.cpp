#include "Material.h"

Color CheckerTexture::sample(float u, float v) const
{
	float sines = std::sin(u * _scale) * std::sin(v * _scale);
	return sines > 0.0f ? _even->sample(u, v) : _odd->sample(u, v);
}

bool Diffuse::scatter(const Vec3& in, const Vec3& normal, Vec3& out) const
{
	out = random_hemisphere(normal);
	return true;
}

bool Metal::scatter(const Vec3& in, const Vec3& normal, Vec3& out) const
{
	Vec3 dir = reflect(in, normal);
	out = dir + _fuzz * random_sphere().normalize();
	return out.Dot(normal) > 0;
}

bool Dielectric::scatter(const Vec3& in, const Vec3& normal, Vec3& out) const
{
	bool isfront = in.Dot(normal) < 0.0f;
	Vec3 scatterNormal = isfront ? normal : -normal;
	float cos_theta = -scatterNormal.Dot(in);
	float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
	float refraction_ratio = isfront ? (1.0f / ir) : ir;
	if (refraction_ratio * sin_theta > 1.0f || reflectance(cos_theta, refraction_ratio) > random_float())
		out = reflect(in, scatterNormal);
	else
		out = refract(in, scatterNormal, refraction_ratio);
	return true;
}