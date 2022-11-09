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

	Sphere s(Point(0.0, 0.0, -10.0), 1.0, Red);
	Camera c(Point(0.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0));
	HitRecord record{ Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0) };


	float ymax = std::tan(fov / 2.0), ymin = -ymax;
	float xmin = ymin * aspect_ratio, xmax = -xmin;
	float xstep = (xmax - xmin) / width, ystep = (ymax - ymin) / height;
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			//Vec3 dir((((float)i + 0.5) / (float)width - 0.5) * 2.0, (((float)j + 0.5) / (float)height - 0.5) * 2.0, 2.0);
			Point pixel(((float)i + 0.5) * xstep + xmin, ((float)j + 0.5) * ystep + ymin, -1.0);
			Vec3 dir = pixel - c.position();
			dir.normalize();
			Ray ray(c.position(), dir);
			if (s.hit(ray, record))
			{
				framebuff.SetPixel(i, j, s.color());
			}
		}
	}
	framebuff.SaveFile("out.bmp");
	return 0;
}