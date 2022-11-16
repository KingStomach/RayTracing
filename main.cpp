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

Color RayTrace(const Ray& in,const Scene& scene, int depth)
{
	if (depth <= 0)
		return Black;

	HitRecord record;
	if (scene.hit(in, 0.001, std::numeric_limits<float>::infinity(), record))
	{
		Ray out(in);
		Color attenuation;
		if (record.object->material()->scatter(in, record, attenuation, out))
			return attenuation * RayTrace(out, scene, depth - 1);
		else
			return Black;
	}
	else
	{
		float t = 0.5 * (in.direction().y() + 1.0);
		return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
	}

}

int main(void)
{
	Bitmap framebuff(width, height);
	Camera camera(Point(0.0, 0.0, 1.0), Vec3(0.0, 0.0, -1.0), Vec3(0.0, 1.0, 0.0), 90.0, aspect_ratio);

	auto material_ground = std::make_shared<Diffuse>(Color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Diffuse>(Color(0.7, 0.3, 0.3));
	auto material_left = std::make_shared<Metal>(Color(0.8, 0.8, 0.8));
	auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2));

	Scene scene;
	scene.addObject(std::make_shared<Sphere>(Point(0.0, -100.5, -1.0), 100, material_ground));
	scene.addObject(std::make_shared<Sphere>(Point(0.0, 0.0, -1.0), 0.5, material_center));
	scene.addObject(std::make_shared<Sphere>(Point(-1.0, 0.0, -1.0), 0.5, material_left));
	scene.addObject(std::make_shared<Sphere>(Point(1.0, 0.0, -1.0), 0.5, material_right));

	clock_t a = clock();
	
	std::mutex mutex;
	concurrency::parallel_for(0, width, [&framebuff, &scene, &camera, &mutex](int i)
		{
			for (int j = 0; j < height; ++j)
			{
				float xoffset, yoffset;
				Color color;
				for (int k = 0; k < 10; k++)
				{
					xoffset = random_float(), yoffset = random_float();
					Ray ray = camera.sample(((float)i + xoffset) / width, ((float)j + yoffset) / height);
					color += RayTrace(ray, scene, 16);
				}

				{
					std::lock_guard<std::mutex> lock(mutex);
					color /= 10.0;
					color ^= (1.0 / 2.2);
					framebuff.SetPixel(i, j, color);
				}
			}
		});

	std::cout << clock() - a;
	framebuff.SaveFile("out.bmp");
	return 0;
}