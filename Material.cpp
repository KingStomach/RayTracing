#include "Material.h"

bool Diffuse::scatter(const Vec3& in, const Vec3& normal, Color& attenuation, Vec3& out) const
{
	out = Vec3::random_hemisphere(normal);
	attenuation = albedo;
	return true;
}

bool Metal::scatter(const Vec3& in, const Vec3& normal, Color& attenuation, Vec3& out) const
{
	Vec3 reflect = Vec3::reflect(in, normal);
	out = reflect + fuzz * Vec3::random_sphere().normalize();
	attenuation = albedo;
	return out.Dot(normal) > 0;
}

bool Dielectric::scatter(const Vec3& in, const Vec3& normal, Color& attenuation, Vec3& out) const
{
	bool isfront = in.Dot(normal) < 0.0f;
	Vec3 scatterNormal = isfront ? normal : -normal;
	float cos_theta = -scatterNormal.Dot(in);
	float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
	float refraction_ratio = isfront ? (1.0f / ir) : ir;
	if (refraction_ratio * sin_theta > 1.0f || reflectance(cos_theta, refraction_ratio) > random_float())
		out = Vec3::reflect(in, scatterNormal);
	else
		out = Vec3::refract(in, scatterNormal, refraction_ratio);
	attenuation = Color(1.0f, 1.0f, 1.0f);
	return true;
}