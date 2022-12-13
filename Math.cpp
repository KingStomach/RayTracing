#include "Math.h"

const float tolerance = 0.001f;

const Color White(1.0f, 1.0f, 1.0f);
const Color Black(0.0f, 0.0f, 0.0f);
const Color Red(1.0f, 0.0f, 0.0f);
const Color Green(0.0f, 1.0f, 0.0f);
const Color Blue(0.0f, 0.0f, 1.0f);

float random_float(float min, float max)
{
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	static std::function<float()> rand_generator =
		std::bind(distribution, generator);
	return (rand_generator() + min) * (max - min);
}

int random_int(int min, int max)
{
	return static_cast<int>(random_float() * (max - min) + min);
}

Vec3 random_sphere()
{
	float theta = random_float(0.0f, PI), phi = random_float(0.0f, PI * 2.0f);
	return Vec3(std::cos(theta) * std::cos(phi), std::cos(theta) * std::sin(phi), std::sin(theta));
}

Vec3 random_hemisphere(const Vec3& v)
{
	Vec3 res = random_sphere();
	if (res.Dot(v) > 0.0f)
		return res;
	else
		return -res;
}

Vec3 random_unit_disk()
{
	float theta = random_float(0.0f, PI * 2.0f), l = random_float();
	return Vec3(l * std::cos(theta), l * std::sin(theta), 0.0f);
}

Color random_color(float min, float max)
{
	return Color(random_float(min, max), random_float(min, max), random_float(min, max));
}

Vec3 reflect(const Vec3& in, const Vec3& normal)
{
	return in - 2.0f * in.Dot(normal) * normal;
}

Vec3 refract(const Vec3& in, const Vec3& normal, float ir_ratio)
{
	float cos_theta1 = -in.Dot(normal), sin_theta1 = std::sqrt(1.0f - cos_theta1 * cos_theta1);
	float sin_theta2 = ir_ratio * sin_theta1, cos_theta2 = std::sqrt(1.0f - sin_theta2 * sin_theta2);
	Vec3 vertical = -cos_theta2 * normal;
	Vec3 horizontal = sin_theta2 * (in + cos_theta1 * normal).normalize();
	return vertical + horizontal;
}

Martix rotate_y(float angle)
{
	Martix m;
	float cos_theta = std::cos(angle * PI / 180.0);
	float sin_theta = std::sin(angle * PI / 180.0);
	m[0][0] = m[2][2] = cos_theta;
	m[0][2] = sin_theta;
	m[2][0] = -sin_theta;
	m[1][1] = 1.0f;
	return m;
}