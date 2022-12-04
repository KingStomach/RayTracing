#include "HitObject.h"

#include <algorithm>

bool AABB::hit(const Ray& ray, float min, float max) const
{
	for (int i = 0; i < 3; i++)
	{
		float inv = 1.0f / ray.direction()[i];
		float t0 = (_pmin[i] - ray.origin()[i]) * inv;
		float t1 = (_pmax[i] - ray.origin()[i]) * inv;
		if (inv < 0.0f)
			std::swap(t0, t1);
		min = std::max(t0, min);
		max = std::min(t1, max);
		if (max <= min)
			return false;
	}
	return true;
}

AABB AABB::merge(const AABB& box) const
{
	Point pmin(std::min(_pmin.x(), box._pmin.x()), std::min(_pmin.y(), box._pmin.y()), std::min(_pmin.z(), box._pmin.z()));
	Point pmax(std::max(_pmax.x(), box._pmax.x()), std::max(_pmax.y(), box._pmax.y()), std::max(_pmax.z(), box._pmax.z()));
	return AABB(pmin, pmax);
}

BVHNode::BVHNode(const std::vector<std::shared_ptr<RenderObject>>& objects)
{
	std::vector<RenderObjectInfo> array;
	for (auto&& object : objects)
		array.emplace_back(object, object->createBox());
	*this = buildBVHTree(array, 0, array.size() - 1);
}

bool BVHNode::hit(const Ray& ray, float min, float max, IntersectInfo& info, std::shared_ptr<RenderObject>& object) const
{
	if (!_box.hit(ray, min, max))
		return false;

	if (isLeaf())
	{
		if (_object->hit(ray, min, max, info))
		{
			object = _object;
			return true;
		}
		else
			return false;
	}

	bool leftHit = left->hit(ray, min, max, info, object);
	if (leftHit)
		max = (info.position - ray.origin()).length();
	bool rightHit = right->hit(ray, min, max, info, object);
	return leftHit || rightHit;
}

BVHNode BVHNode::buildBVHTree(std::vector<RenderObjectInfo>& objects, int left, int right) const
{
	if (left == right)
		return BVHNode(objects[left].first, objects[left].second);

	int mid = (left + right) / 2;
	int axis = random_int(0, 3);
	switch (axis)
	{
	case 0:
		std::sort(objects.begin() + left, objects.begin() + right + 1,
			[](const RenderObjectInfo& o1, const RenderObjectInfo& o2)->bool
			{ return o1.second.center().x() < o2.second.center().x(); });
		break;
	case 1:
		std::sort(objects.begin() + left, objects.begin() + right + 1,
			[](const RenderObjectInfo& o1, const RenderObjectInfo& o2)->bool
			{ return o1.second.center().y() < o2.second.center().y(); });
		break;
	case 2:
		std::sort(objects.begin() + left, objects.begin() + right + 1,
			[](const RenderObjectInfo& o1, const RenderObjectInfo& o2)->bool
			{ return o1.second.center().z() < o2.second.center().z(); });
		break;
	default:
		break;
	}

	BVHTree leftTree = buildBVHTree(objects, left, mid);
	BVHTree rightTree = buildBVHTree(objects, mid + 1, right);
	BVHTree tree(leftTree._box.merge(rightTree._box));
	tree.left = std::make_shared<BVHNode>(leftTree);
	tree.right = std::make_shared<BVHNode>(rightTree);
	return tree;
}

bool Sphere::hit(const Ray& ray, float min, float max, IntersectInfo& info) const
{
	Vec3 oc = ray.origin() - center();
	float a = ray.direction().Dot(ray.direction());
	float b = 2.0f * ray.direction().Dot(oc);
	float c = oc.Dot(oc) - radius() * radius();
	float delta = b * b - 4.0f * a * c;

	if (delta < 0)
		return false;
	float t = std::sqrt(delta);
	float t1 = (-b - t) * 0.5f / a;
	if (t1 < min)
		t1 = (-b + t) * 0.5f / a;
	if (t1 < min || t1 > max)
		return false;

	info.position = ray.at(t1);
	info.normal = (info.position - center()).normalize();
	auto uv = getUV(info.normal);
	info.u = uv.first;
	info.v = uv.second;
	return true;
}

AABB Sphere::createBox() const
{
	Vec3 dir(_radius, _radius, _radius);
	return AABB(_center - dir, _center + dir);
}

std::pair<float, float> Sphere::getUV(const Vec3& normal) const
{
	std::pair<float, float> uv;
	float phi = std::asin(normal[2]);
	float theta = std::acos(normal[0] / std::cos(phi));
	return std::make_pair<float, float>(theta / 2.0 / PI, phi / PI);
}

bool MovingSphere::hit(const Ray& ray, float min, float max, IntersectInfo& info) const
{
	Vec3 oc = ray.origin() - center(ray.time());
	float a = ray.direction().Dot(ray.direction());
	float b = 2.0f * ray.direction().Dot(oc);
	float c = oc.Dot(oc) - radius() * radius();
	float delta = b * b - 4.0f * a * c;

	if (delta < 0)
		return false;
	float t = std::sqrt(delta);
	float t1 = (-b - t) * 0.5f / a;
	if (t1 < min)
		t1 = (-b + t) * 0.5f / a;
	if (t1 < min || t1 > max)
		return false;

	info.position = ray.at(t1);
	info.normal = (info.position - center(ray.time())).normalize();
	auto uv = getUV(info.normal);
	info.u = uv.first;
	info.v = uv.second;
	return true;
}

bool RectangleXY::hit(const Ray& ray, float min, float max, IntersectInfo& info) const
{
	float t = (z - ray.origin().z()) / ray.direction().z();
	if (t < min || t > max)
		return false;

	float x = ray.origin().x() + t * ray.direction().x();
	float y = ray.origin().y() + t * ray.direction().y();
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;

	info.position = Point(x, y, z);
	info.normal = ray.origin().z() > z ? Vec3(0.0f, 0.0f, 1.0f) : Vec3(0.0f, 0.0f, -1.0f);
	info.u = (x - x0) / (x1 - x0);
	info.v = (y - y0) / (y1 - y0);
	return true;
}

AABB RectangleXY::createBox() const
{
	return AABB(Point(x0, y0, z - 0.001f), Point(x1, y1, z + 0.001f));
}

bool RectangleYZ::hit(const Ray& ray, float min, float max, IntersectInfo& info) const
{
	float t = (x - ray.origin().x()) / ray.direction().x();
	if (t < min || t > max)
		return false;

	float y = ray.origin().y() + t * ray.direction().y();
	float z = ray.origin().z() + t * ray.direction().z();
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;

	info.position = Point(x, y, z);
	info.normal = ray.origin().x() > x ? Vec3(1.0f, 0.0f, 0.0f) : Vec3(-1.0f, 0.0f, 0.0f);
	info.u = (y - y0) / (y1 - y0);
	info.v = (z - z0) / (z1 - z0);
	return true;
}

AABB RectangleYZ::createBox() const
{
	return AABB(Point(x - 0.001f, y0, z0), Point(x + 0.001f, y1, z1));
}

bool RectangleXZ::hit(const Ray& ray, float min, float max, IntersectInfo& info) const
{
	float t = (y - ray.origin().y()) / ray.direction().y();
	if (t < min || t > max)
		return false;

	float x = ray.origin().x() + t * ray.direction().x();
	float z = ray.origin().z() + t * ray.direction().z();
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;

	info.position = Point(x, y, z);
	info.normal = ray.origin().y() > y ? Vec3(0.0f, 1.0f, 0.0f) : Vec3(0.0f, -1.0f, 0.0f);
	info.u = (x - x0) / (x1 - x0);
	info.v = (z - z0) / (z1 - z0);
	return true;
}

AABB RectangleXZ::createBox() const
{
	return AABB(Point(x0, y - 0.001f, z0), Point(x1, y + 0.001f, z1));
}

Cuboid::Cuboid(const Point& p0, const Point& p1, const std::shared_ptr<Material>& material)
	: _pmin(p0), _pmax(p1), RenderObject(material)
{
	scene.addObject(std::make_shared<RectangleXY>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material));
	scene.addObject(std::make_shared<RectangleXY>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material));

	scene.addObject(std::make_shared<RectangleXZ>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material));
	scene.addObject(std::make_shared<RectangleXZ>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material));

	scene.addObject(std::make_shared<RectangleYZ>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material));
	scene.addObject(std::make_shared<RectangleYZ>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material));

	scene.buildBVHTree();
}

bool Cuboid::hit(const Ray& ray, float min, float max, IntersectInfo& info) const
{
	std::shared_ptr<RenderObject> object;
	return scene.hit(ray, min, max, info, object);
}

AABB Cuboid::createBox() const
{
	return AABB(_pmin, _pmax);
}

bool Scene::hit(const Ray& ray, float min, float max, IntersectInfo& info, std::shared_ptr<RenderObject>& object) const
{
	return _tree.hit(ray, min, max, info, object);
	/*
	float closest = max;
	bool ishit = false;
	for (auto&& o : _objects)
	{
		if (o->hit(ray, min, closest, info))
		{
			ishit = true;
			closest = (info.position - ray.origin()).length();
			object = o;
		}
	}
	return ishit;*/
}
