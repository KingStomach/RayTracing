#include<cmath>
#include <iostream>
#include <ppl.h>

#include "HitObject.h"
#include "Material.h"
#include "Math.h"
#include "Render.h"

const int width = 512;
const int height = 384;

Scene random_scene()
{
	Scene scene;

	auto checker = std::make_shared<CheckerTexture>(Color(0.2f, 0.3f, 0.1f), Color(0.9f, 0.9f, 0.9f), 10000.0f);
	auto ground_material = std::make_shared<Diffuse>(checker);
	scene.addObject(std::make_shared<Sphere>(Point(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_float();
			Point center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

			if ((center - Point(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8f) {
					// diffuse
					auto albedo = random_color() * random_color();
					sphere_material = std::make_shared<Diffuse>(albedo);
					//scene.addObject(std::make_shared<Sphere>(center, 0.2f, sphere_material));
					scene.addObject(std::make_shared<MovingSphere>(center, 0.2f, sphere_material, Vec3(0.0f, random_float(0.0f, 0.5f), 0.0f)));
				}
				else if (choose_mat < 0.95f) {
					// metal
					auto albedo = random_color(0.5f, 1.0f);
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

Scene two_sphere()
{
	Scene scene;

	auto checker = std::make_shared<CheckerTexture>(Color(0.2f, 0.3f, 0.1f), Color(0.9f, 0.9f, 0.9f), 500.0f);
	auto material = std::make_shared<Diffuse>(checker);

	scene.addObject(std::make_shared<Sphere>(Point(0.0f, -10.0f, 0.0f), 10.0f, material));
	scene.addObject(std::make_shared<Sphere>(Point(0.0f, 10.0f, 0.0f), 10.0f, material));

	return scene;
}

Scene simple_light()
{
	Scene scene;
	auto material = std::make_shared<Diffuse>(Color(0.2f, 0.3f, 0.1f));

	scene.addObject(std::make_shared<Sphere>(Point(0.0f, -1000.0f, 0.0f), 1000.0f, material));
	scene.addObject(std::make_shared<Sphere>(Point(0.0f, 2.0f, 0.0f), 2.0f, material));

	auto difflight = std::make_shared<Emissive>(Color(4.0f, 4.0f, 4.0f));
	scene.addObject(std::make_shared<RectangleXY>(3.0f, 5.0f, 1.0f, 3.0f, -2.0f, difflight));

	return scene;
}

Scene cornell_box()
{
	Scene scene;

	auto red = std::make_shared<Diffuse>(Color(0.65f, 0.05f, 0.05f));
	auto white = std::make_shared<Diffuse>(Color(0.73f, 0.73f, 0.73f));
	auto green = std::make_shared<Diffuse>(Color(0.12f, 0.45f, 0.15f));
	auto light = std::make_shared<Emissive>(Color(15.0f, 15.0f, 15.0f));

	scene.addObject(std::make_shared<RectangleYZ>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, red));
	scene.addObject(std::make_shared<RectangleYZ>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, green));
	scene.addObject(std::make_shared<RectangleXZ>(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light));
	scene.addObject(std::make_shared<RectangleXZ>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white));
	scene.addObject(std::make_shared<RectangleXZ>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	scene.addObject(std::make_shared<RectangleXY>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));

	auto box1 = std::make_shared<Cuboid>(Point(210.0f, 0.0f, 165.0f), Point(375.0f, 165.0f, 330.0f), white);
	auto box2 = std::make_shared<Cuboid>(Point(175.0f, 0.0f, 220.0f), Point(340.0f, 330.0f, 385.0f), white);
	scene.addObject(std::make_shared<Rotate_Y>(box1, 18));
	scene.addObject(std::make_shared<Rotate_Y>(box2, -15));

	return scene;
}

Color RayTrace(const Ray& in,const Scene& scene, int depth)
{
	if (depth <= 0)
		return Black;

	IntersectInfo info;
	std::shared_ptr<RenderObject> object;
	if (scene.hit(in, 0.001f, std::numeric_limits<float>::infinity(), info, object))
	{
		Vec3 out_dir;
		if (object->material()->scatter(in.direction(), info.normal, out_dir))
		{
			Color attenuation = object->material()->attenuation(info.u, info.v);
			return attenuation * RayTrace(Ray(info.position, out_dir, in.time()), scene, depth - 1);
		}
		else
			return object->material()->emit(info.u, info.v);
	}
	else
	{
		//float t = 0.5f * (in.direction().y + 1.0f);
		//return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
		return Black;
	}

}

int main(void)
{
	float fov = 90.0f;
	float aspect_ratio = (float)width / height;
	int samples_per_pixel = 100;
	Bitmap framebuff(width, height);
	Point position, lookat;
	Vec3 up(0.0f, 1.0f, 0.0f);

	Scene scene;

	switch (4)
	{
	case 0:
	{
		position = Point(0.0f, 0.0f, 1.0f);
		lookat = Point(0.0f, 0.0f, -1.0f);

		auto material_ground = std::make_shared<Diffuse>(Color(0.8f, 0.8f, 0.0f));
		auto material_center = std::make_shared<Diffuse>(Color(0.1f, 0.2f, 0.5f));
		auto material_left = std::make_shared<Dielectric>(1.5f);
		auto material_right = std::make_shared<Metal>(Color(0.8f, 0.6f, 0.2f), 0.0f);

		scene.addObject(std::make_shared<Sphere>(Point(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
		scene.addObject(std::make_shared<Sphere>(Point(0.0f, 0.0f, -1.0f), 0.5f, material_center));
		scene.addObject(std::make_shared<Sphere>(Point(1.0f, 0.0f, -1.0f), 0.5f, material_left));
		scene.addObject(std::make_shared<Sphere>(Point(-1.0f, 0.0f, -1.0f), 0.5f, material_right));

		break;
	}
	case 1:
	{
		position = Point(13.0f, 2.0f, 3.0f);
		lookat = Point(0.0f, 0.0f, 0.0f);
		fov = 60.0f;

		scene = random_scene();

		break;
	}
	case 2:
	{
		position = Point(13.0f, 2.0f, 3.0f);
		lookat = Point(0.0f, 0.0f, 0.0f);
		fov = 60.0f;

		scene = two_sphere();
		break;
	}
	case 3:
	{
		position = Point(26.0f, 3.0f, 6.0f);
		lookat = Point(0.0f, 2.0f, 0.0f);
		samples_per_pixel = 400;
		fov = 60.0f;

		scene = simple_light();
		break;
	}
	case 4:
	{
		position = Point(278.0f, 278.0f, -800.0f);
		lookat = Point(278.0f, 278.0f, 0.0f);
		samples_per_pixel = 200;
		fov = 75.0f;

		scene = cornell_box();
		break;
	}
	default:
		break;
	}

	Camera camera(position, lookat, up, fov, aspect_ratio);
	scene.buildBVHTree();

	clock_t start_time = clock();
	
	std::mutex buffer_mutex;
	std::mutex progress_mutex;
	int finish_line = 0;
	
	concurrency::parallel_for(0, width, [&framebuff,&samples_per_pixel , &scene, &camera, &buffer_mutex, &progress_mutex, &finish_line](int i)
		{
			for (int j = 0; j < height; ++j)
			{
				float xoffset, yoffset;
				Color color;
				for (int k = 0; k < samples_per_pixel; k++)
				{
					xoffset = random_float(), yoffset = random_float();
					Ray ray = camera.sample(((float)i + xoffset) / width, ((float)j + yoffset) / height);
					color += RayTrace(ray, scene, 50);
				}

				color /= (float)samples_per_pixel;
				color ^= (1.0f / 2.2f);
				{
					std::lock_guard<std::mutex> lock(buffer_mutex);
					framebuff.SetPixel(i, j, color);
				}
			}

			{
				std::lock_guard<std::mutex> lock(progress_mutex);
				finish_line++;
				std::cout << "Drawing:" + std::to_string((float)finish_line / width * 100.0f) + "%" << std::endl;
			}
		});
	std::cout << clock() - start_time;
	framebuff.SaveFile("out.bmp");
	return 0;
}