#include <iostream>
#include<cmath>

#include "Hittable/Sphere.h"
#include "Render/Bitmap.h"
#include "Render/Camera.h"
#include "Render/Ray.h"

const int width = 1024;
const int height = 768;
const float fov = 45.0;
const float aspect_ratio = (float)width / height;

int main(void)
{
	Bitmap framebuff(width, height);

	Sphere s(Point(0.0, 0.0, -1.0), 0.5, Red);
	Sphere s1(Point(0.0, -100.5, -1.0), 100, Green);
	Sphere SkySphere(Point(0.0, 0.0, 0.0), 1000, Blue);
	Camera c(Point(0.0, 0.0, 1.0), Vec3(0.0, 0.0, -1.0), Vec3(0.0, 1.0, 0.0), 45.0, aspect_ratio);
	HitRecord record;

	float ymax = std::tan(fov / 2.0), ymin = -ymax;
	float xmin = ymin * aspect_ratio, xmax = -xmin;
	float xstep = (xmax - xmin) / width, ystep = (ymax - ymin) / height;
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			Ray ray = c.sample(((float)i + 0.5) / width, ((float)j + 0.5) / height);
			if (s.hit(ray, record))
			{
				framebuff.SetPixel(i, j, s.color());
			}
			else if (s1.hit(ray, record))
			{
				framebuff.SetPixel(i, j, s1.color());
			}
			else
			{
				framebuff.SetPixel(i, j, SkySphere.color());
			}
		}
	}
	framebuff.SaveFile("out.bmp");
	return 0;
}