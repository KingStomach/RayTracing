#ifndef _HITOBJECT_H_
#define _HITOBJECT_H_

#include "../Array3/Color.h"
#include "../Array3/Point.h"
#include "../Render/Ray.h"

struct HitRecord
{
	Point position;
	Vec3 normal;
};

class HitObject
{
public:
	HitObject(const Color& color) : c(color) {}

	virtual bool hit(const Ray& ray, HitRecord& record) = 0;

	inline Color color() const { return c; }

private:
	Color c;
};

#endif