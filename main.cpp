#include <iostream>
#include<cmath>

#include "Array3.h"
#include "HitObject.h"
#include "Render.h"

const int width = 1024;
const int height = 768;
const float fov = 45.0;
const float aspect_ratio = (float)width / height;

Color RayTrace(const Ray& ray,const Sphere& s1, const Sphere& s2, int depth)
{
	if (depth <= 0)
		return Black;

	HitRecord record1, record2, record;
	bool hit1 = s1.hit(ray, record1), hit2 = s2.hit(ray, record2);
	if (!hit1 && !hit2)
	{
		float t = 0.5 * (ray.direction().y() + 1.0);
		return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
	}
	else if (hit1 || hit2)
		record = hit1 ? record1 : record2;
	else
		record = record1.t > record2.t ? record2 : record1;

	Vec3 targetdir = Vec3::random_hemisphere(record.normal);
	return 0.5 * RayTrace(Ray(record.position, targetdir), s1, s2, depth - 1);
}

int main(void)
{
	Bitmap framebuff(width, height);

	Sphere s1(Point(0.0, 0.0, -1.0), 0.5);
	Sphere s2(Point(0.0, -100.5, -1.0), 100);
	Camera c(Point(0.0, 0.0, 1.0), Vec3(0.0, 0.0, -1.0), Vec3(0.0, 1.0, 0.0), 90.0, aspect_ratio);
	//HitRecord record;

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			Ray ray = c.sample(((float)i + 0.5) / width, ((float)j + 0.5) / height);
			Color color;
			for (int i = 0; i < 10; i++)
			{
				color += RayTrace(ray, s1, s2, 16);
			}
			framebuff.SetPixel(i, j, color / 10.0);
			/*
			if (s1.hit(ray, record))
			{
				Color a = 0.5 * (record.normal + Color(1.0, 1.0, 1.0));
				framebuff.SetPixel(i, j, a);
			}
			else if (s2.hit(ray, record))
			{
				Color a = 0.5 * (record.normal + Color(1.0, 1.0, 1.0));
				framebuff.SetPixel(i, j, a);
			}
			else
			{
				auto t = 0.5 * (ray.direction().y() + 1.0);
				Color c = (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
				framebuff.SetPixel(i, j, c);
			}*/
		}
	}
	framebuff.SaveFile("out.bmp");
	return 0;
}