#include<cmath>
#include <iostream>
#include <ppl.h>

#include "Array3.h"
#include "HitObject.h"
#include "Material.h"
#include "Render.h"

const int width = 1024;
const int height = 768;
const float fov = 45.0f;
const float aspect_ratio = (float)width / height;


Scene random_scene()
{
	Scene scene;

	auto ground_material = std::make_shared<Diffuse>(Color(0.5f, 0.5f, 0.5f));
	scene.addObject(std::make_shared<Sphere>(Point(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_float();
			Point center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

			if ((center - Point(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8f) {
					// diffuse
					auto albedo = Color::random() * Color::random();
					sphere_material = std::make_shared<Diffuse>(albedo);
					scene.addObject(std::make_shared<Sphere>(center, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95f) {
					// metal
					auto albedo = Color::random(0.5f, 1.0f);
					float fuzz = random_float(0.0f, 0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					scene.addObject(std::make_shared<Sphere>(center, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(1.5);
					scene.addObject(std::make_shared<Sphere>(center, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5);
	scene.addObject(std::make_shared<Sphere>(Point(0.0f, 1.0f, 0.0f), 1.0f, material1));

	auto material2 = std::make_shared<Diffuse>(Color(0.4f, 0.2f, 0.1f));
	scene.addObject(std::make_shared<Sphere>(Point(-4.0f, 1.0f, 0.0f), 1.0f, material2));

	auto material3 = std::make_shared<Metal>(Color(0.7f, 0.6f, 0.5f), 0.0f);
	scene.addObject(std::make_shared<Sphere>(Point(4.0f, 1.0f, 0.0f), 1.0f, material3));

	return scene;
}

Color RayTrace(const Ray& in,const Scene& scene, int depth)
{
	if (depth <= 0)
		return Black;

	HitRecord record;
	if (scene.hit(in, 0.001f, std::numeric_limits<float>::infinity(), record))
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
		float t = 0.5f * (in.direction().y() + 1.0f);
		return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
	}

}

int main(void)
{
	Bitmap framebuff(width, height);
	Camera camera(Point(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f), 90.0f, aspect_ratio);
	//Camera camera(Point(13.0f, 2.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0, 1.0f, 0), 60.0, aspect_ratio);
	
	auto material_ground = std::make_shared<Diffuse>(Color(0.8f, 0.8f, 0.0f));
	auto material_center = std::make_shared<Diffuse>(Color(0.1f, 0.2f, 0.5f));
	auto material_left = std::make_shared<Dielectric>(1.5f);
	auto material_right = std::make_shared<Metal>(Color(0.8f, 0.6f, 0.2f), 0.0f);

	Scene scene;
	scene.addObject(std::make_shared<Sphere>(Point(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
	scene.addObject(std::make_shared<Sphere>(Point(0.0f, 0.0f, -1.0f), 0.5f, material_center));
	scene.addObject(std::make_shared<Sphere>(Point(1.0f, 0.0f, -1.0f), 0.5f, material_left));
	scene.addObject(std::make_shared<Sphere>(Point(-1.0f, 0.0f, -1.0f), 0.5f, material_right));

	//Scene scene = random_scene();

	clock_t start_time = clock();
	
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
					color += RayTrace(ray, scene, 50);
				}

				color /= 10.0f;
				color ^= (1.0f / 2.2f);
				{
					std::lock_guard<std::mutex> lock(mutex);
					framebuff.SetPixel(i, j, color);
				}
			}
		});

	//std::cout << clock() - start_time;
	framebuff.SaveFile("out.bmp");
	return 0;
}