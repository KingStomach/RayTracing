#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "HitObject.h"

class Material
{
public:
	//virtual bool scatter(const Ray& in, Ray& out) const = 0;
};

class Diffuse : public Material
{
public:
	//virtual bool scatter(const Ray& in, Ray& out) const;
};


#endif