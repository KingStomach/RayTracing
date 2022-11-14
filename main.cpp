#include<cmath>
#include <iostream>
#include <ppl.h>

#include "Array3.h"
#include "HitObject.h"
#include "Material.h"
#include "Render.h"

const int width = 1024;
const int height = 768;
const float fov = 45.0;
const float aspect_ratio = (float)width / height;

Color RayTrace(const Ray& in,const Sphere& s1, const Sphere& s2, int depth)
{
	if (depth <= 0)
		return Black;

	HitRecord record1, record2, record;
	bool hit1 = s1.hit(in, record1), hit2 = s2.hit(in, record2);
	if (!hit1 && !hit2)
	{
		float t = 0.5 * (in.direction().y() + 1.0);
		return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
	}
	else if (hit1 || hit2)
		record = hit1 ? record1 : record2;
	else
		record = record1.t > record2.t ? record2 : record1;

	Ray out(in);
	Color attenuation;
	s1.material().get()->scatter(in, record, attenuation, out);
	return attenuation * RayTrace(out, s1, s2, depth - 1);
}

int main(void)
{
	Bitmap framebuff(width, height);
	auto diffusematerial = std::make_shared<Diffuse>(Color(0.5, 0.5, 0.5));
	Sphere s1(Point(0.0, 0.0, -1.0), 0.5, diffusematerial);
	Sphere s2(Point(0.0, -100.5, -1.0), 100, diffusematerial);
	Camera c(Point(0.0, 0.0, 1.0), Vec3(0.0, 0.0, -1.0), Vec3(0.0, 1.0, 0.0), 90.0, aspect_ratio);

	clock_t a = clock();
	
	std::mutex m;
	concurrency::parallel_for(0, width, [&framebuff, &m, &s1, &s2, &c](int i)
		{
			for (int j = 0; j < height; ++j)
			{
				float xoffset, yoffset;
				Color color;
				for (int k = 0; k < 10; k++)
				{
					xoffset = random_float(), yoffset = random_float();
					Ray ray = c.sample(((float)i + xoffset) / width, ((float)j + yoffset) / height);
					color += RayTrace(ray, s1, s2, 16);
				}

				{
					std::lock_guard<std::mutex> lock(m);
					color /= 10.0;
					color ^= (1.0 / 2.2);
					framebuff.SetPixel(i, j, color);
				}
			}
		});
	/*
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			float xoffset, yoffset;
			Color color;
			for (int k = 0; k < 10; k++)
			{
				xoffset = random_float() - 0.5, yoffset = random_float() - 0.5;
				Ray ray = c.sample(((float)i + xoffset + 0.5) / width, ((float)j + yoffset + 0.5) / height);
				color += RayTrace(ray, s1, s2, 16);
			}
			framebuff.SetPixel(i, j, color / 10.0);
		}
	}*/
	/*
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			Color color((float)i / width, (float)i / width, (float)i / width);
			color ^= (1.0/2.2);
			framebuff.SetPixel(i, j, color);
		}
	}*/
	std::cout << clock() - a;
	framebuff.SaveFile("out.bmp");
	return 0;
}