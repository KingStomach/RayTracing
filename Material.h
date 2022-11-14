#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "HitObject.h"

class Material
{
public:
	virtual bool scatter(const Ray& in, const HitRecord& record, Color& attenuation, Ray& out) const = 0;
};

class Diffuse : public Material
{
public:
	Diffuse(const Color& albedo) : albedo(albedo) {}

	bool scatter(const Ray& in, const HitRecord& record, Color& attenuation, Ray& out) const override;
private:
	Color albedo;
};


#endif