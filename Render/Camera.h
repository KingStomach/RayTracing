#ifndef _CARMERA_H_
#define _CARMERA_H_	

#include "../Array3/Point.h"
#include "Ray.h"

class Camera
{
public:
	Camera(const Point& position, const Vec3& lookat, const Vec3& up, float fov, float aspect_ratio);

	inline Point position() const { return p; }

	Ray sample(float s, float v);

private:
	Point p;
	Vec3 lookat;
	Vec3 up;
	float yoffset;
	float xoffset;
};


#endif