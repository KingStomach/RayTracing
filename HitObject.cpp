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
	return true;
}

AABB Sphere::createBox() const
{
	Vec3 dir(_radius, _radius, _radius);
	return AABB(_center - dir, _center + dir);
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
	return true;
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
