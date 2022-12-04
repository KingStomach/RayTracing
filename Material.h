#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "HitObject.h"

class Texutre
{
public:
	virtual Color sample(float u, float v) const = 0;
};

class SoildTexutre : public Texutre
{
public:
	explicit SoildTexutre(const Color& color) : _color(color) {}

	Color sample(float u, float v) const override { return _color; }

private:
	Color _color;
};

class CheckerTexture : public Texutre
{
public:
	explicit CheckerTexture(const std::shared_ptr<Texutre>& even, const std::shared_ptr<Texutre>& odd, float scale = 1.0f) : _even(even), _odd(odd), _scale(scale) {}
	explicit CheckerTexture(const Color& c1, const Color& c2, float scale = 1.0f) : _even(std::make_shared<SoildTexutre>(c1)), _odd(std::make_shared<SoildTexutre>(c2)), _scale(scale) {}

	Color sample(float u, float v) const override;

private:
	std::shared_ptr<Texutre> _even;
	std::shared_ptr<Texutre> _odd;
	float _scale;
};

class Material
{
public:
	virtual bool scatter(const Vec3& in, const Vec3& normal, Vec3& out) const = 0;
	virtual Color attenuation(float u, float v) const { return White; }
	virtual Color emit(float u, float v) const { return Black; }
};

class Diffuse : public Material
{
public:
	explicit Diffuse(const Color& albedo) : _albedo(std::make_shared<SoildTexutre>(albedo)) {}
	explicit Diffuse(const std::shared_ptr<Texutre>& texture) : _albedo(texture) {}

	bool scatter(const Vec3& in, const Vec3& normal, Vec3& out) const override;
	Color attenuation(float u, float v) const override { return _albedo->sample(u, v); }

private:
	std::shared_ptr<Texutre> _albedo;
};

class Metal : public Material
{
public:
	explicit Metal(const Color& albedo, float fuzz) : _albedo(albedo), _fuzz(fuzz) {}

	bool scatter(const Vec3& in, const Vec3& normal, Vec3& out) const override;
	Color attenuation(float u, float v) const override { return _albedo; }

private:
	Color _albedo;
	float _fuzz;
};

class Dielectric : public Material
{
public:
	explicit Dielectric(float ir) : ir(ir) {}

	bool scatter(const Vec3& in, const Vec3& normal, Vec3& out) const override;

private:
	float ir;

	inline static float reflectance(float cos_theta, float refraction_index)
	{
		float r0 = (refraction_index - 1.0f) / (refraction_index + 1.0f);
		r0 = r0 * r0;
		return r0 + (1.0f - r0) * std::pow((1 - cos_theta), 5);
	}
};

class Emissive : public Material
{
public:
	explicit Emissive(const Color& color) : _emit(std::make_shared<SoildTexutre>(color)) {}
	explicit Emissive(const std::shared_ptr<Texutre>& texture) : _emit(texture) {}

	bool scatter(const Vec3& in, const Vec3& normal, Vec3& out) const override { return false; }
	Color emit(float u, float v) const { return _emit->sample(u, v); }

private:
	std::shared_ptr<Texutre> _emit;
};


#endif