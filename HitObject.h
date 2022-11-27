#ifndef _HITOBJECT_H_
#define _HITOBJECT_H_

#include "Array3.h"

class Material;
class RenderObject;

struct HitRecord
{
	Point position;
	Vec3 normal;
	std::shared_ptr<RenderObject> object;
};

class Ray
{
public:
	Ray() {}
	Ray(const Point& point, const Vec3& dir) : _origin(point), _dir(dir) {}

	Point origin() const { return _origin; }
	Vec3 direction() const { return _dir; }
	Point at(float t) const { return _origin + _dir * t; }

private:
	Point _origin;
	Vec3 _dir;
};

class BoundingBox
{
public:
	virtual bool hit(const Ray& ray, float min, float max) const = 0;

	virtual Point center() const = 0;
};

class AABB : public BoundingBox
{
public:
	AABB() : _pmin(Point(0.0f, 0.0f, 0.0f)), _pmax(Point(0.0f, 0.0f, 0.0f)) {}
	explicit AABB(const Point& p1, const Point& p2) : _pmin(p1), _pmax(p2) {}

	bool hit(const Ray& ray, float min, float max) const override;
	Point center() const override { return _pmin + (_pmax - _pmin) * 0.5f; }

	AABB merge(const AABB& box) const;

private:
	Point _pmin;
	Point _pmax;
};

class RenderObject
{
public:
	std::shared_ptr<Material> material() const { return _material; }

	virtual bool hit(const Ray& ray, float min, float max, HitRecord& record) const = 0;
	virtual AABB createBox() const = 0;

protected:
	explicit RenderObject(const std::shared_ptr<Material>& material) : _material(material) {}

protected:
	std::shared_ptr<Material> _material;
};

class Sphere : public RenderObject
{
public:
	explicit Sphere(const Point& center, float radius, const std::shared_ptr<Material>& material)
		: _center(center), _radius(radius), RenderObject(material) {}

	bool hit(const Ray& ray, float min, float max, HitRecord& record) const override;
	AABB createBox() const override;

	Point center() { return _center; }
	const Point& center() const { return _center; }
	float radius() const { return _radius; }

protected:
	Point _center;
	float _radius;
};

typedef std::pair<std::shared_ptr<RenderObject>, AABB> RenderObjectInfo;

class BVHNode
{
public:
	BVHNode() {}
	explicit BVHNode(const std::shared_ptr<RenderObject>& object) : _object(object), _box(object->createBox()) {}
	explicit BVHNode(const AABB& box) : _box(box) {}
	explicit BVHNode(const std::shared_ptr<RenderObject>& object, const AABB& box) : _object(object), _box(box) {}
	explicit BVHNode(const std::vector<std::shared_ptr<RenderObject>>& objects);

	bool hit(const Ray& ray, float min, float max, HitRecord& record) const;
	bool isLeaf() const { return !left && !right; }

	std::shared_ptr<BVHNode> left;
	std::shared_ptr<BVHNode> right;

private:
	AABB _box;
	std::shared_ptr<RenderObject> _object;

	BVHNode buildBVHTree(std::vector<RenderObjectInfo>& objects, int left, int right) const;
};

typedef BVHNode BVHTree;

class Scene
{
public:
	Scene() {}

	inline void addObject(const std::shared_ptr<RenderObject>& object) { _objects.emplace_back(object); }
	inline void clear() { _objects.clear(); }

	bool hit(const Ray& ray, float min, float max, HitRecord& record) const;
	void buildBVHTree() { _tree = BVHTree(_objects); }

private:
	BVHTree _tree;
	std::vector<std::shared_ptr<RenderObject>> _objects;
};

#endif