#include "Material.h"

bool Diffuse::scatter(const Ray& in, const HitRecord& record, Color& attenuation, Ray& out) const
{
	out = Ray(record.position, Vec3::random_hemisphere(record.normal));
	attenuation = albedo;
	return true;
}

bool Metal::scatter(const Ray& in, const HitRecord& record, Color& attenuation, Ray& out) const
{
	Vec3 reflect = Vec3::reflect(in.direction(), record.normal);
	out = Ray(record.position, (reflect + fuzz * Vec3::random_sphere()).normalize());
	attenuation = albedo;
	return out.direction().Dot(record.normal) > 0;
}

bool Dielectric::scatter(const Ray& in, const HitRecord& record, Color& attenuation, Ray& out) const
{
	Vec3 normal = record.isfront ? record.normal : -record.normal;
	float cos_theta = -normal.Dot(in.direction());
	float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
	float refraction_ratio = record.isfront ? (1.0f / ir) : ir;
	Vec3 out_direction;
	if (refraction_ratio * sin_theta > 1.0f || reflectance(cos_theta, refraction_ratio) > random_float())
	//if (refraction_index * sin_theta > 1.0f)
		out_direction = Vec3::reflect(in.direction(), normal);
	else
		out_direction = Vec3::refract(in.direction(), normal, refraction_ratio);

	out = Ray(record.position, out_direction);
	attenuation = Color(1.0f, 1.0f, 1.0f);
	return true;
}