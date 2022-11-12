#include "Camera.h"

Camera::Camera(const Point& position, const Vec3& lookat, const Vec3& up, float fov, float aspect_ratio)
	: p(position), lookat(lookat), up(up), yoffset(std::tan(fov / 2.0)), xoffset(yoffset* aspect_ratio){}

Ray Camera::sample(float s, float v)
{
	Vec3 dir = lookat - p;
	dir.normalize();
	Vec3 offset = (s - 0.5) * xoffset * (up.cross(dir)) + (v - 0.5) * yoffset * up;
	dir = dir + offset;
	dir.normalize();
	return Ray(p, dir);
}