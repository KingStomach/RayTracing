#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "HitObject.h"

class Material
{
public:
	virtual bool scatter(const Vec3& in, const Vec3& normal, Color& attenuation, Vec3& out) const = 0;
};

class Diffuse : public Material
{
public:
	Diffuse(const Color& albedo) : albedo(albedo) {}

	bool scatter(const Vec3& in, const Vec3& normal, Color& attenuation, Vec3& out) const override;
private:
	Color albedo;
};

class Metal : public Material
{
public:
	Metal(const Color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {}

	bool scatter(const Vec3& in, const Vec3& normal, Color& attenuation, Vec3& out) const override;
private:
	Color albedo;
	float fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(float ir) : ir(ir) {}

	bool scatter(const Vec3& in, const Vec3& normal, Color& attenuation, Vec3& out) const override;
private:
	float ir;

	inline static float reflectance(float cos_theta, float refraction_index)
	{
		float r0 = (refraction_index - 1.0f) / (refraction_index + 1.0f);
		r0 = r0 * r0;
		return r0 + (1.0f - r0) * std::pow((1 - cos_theta), 5);
	}
};


#endif