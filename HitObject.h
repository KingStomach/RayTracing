#ifndef _HITOBJECT_H_
#define _HITOBJECT_H_

#include "Math.h"

class Material;
class RenderObject;

struct IntersectInfo
{
	Point position;
	Vec3 normal;
	float u;
	float v;
};

class Ray
{
public:
	explicit Ray(const Point& point, const Vec3& dir, float time = 0.0f) : _origin(point), _dir(dir), _time(time) {}

	Point origin() const { return _origin; }
	Vec3 direction() const { return _dir; }
	float time() const { return _time; }
	Point at(float t) const { return _origin + _dir * t; }

private:
	Point _origin;
	Vec3 _dir;
	float _time;
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
	AABB transform(const Martix& m) const;

	AABB merge(const AABB& box) const;

private:
	Point _pmin;
	Point _pmax;
};

class RenderObject
{
public:
	std::shared_ptr<Material> material() const { return _material; }

	virtual bool hit(const Ray& ray, float min, float max, IntersectInfo& info) const = 0;
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

	bool hit(const Ray& ray, float min, float max, IntersectInfo& info) const override;
	AABB createBox() const override;

	Point center() { return _center; }
	const Point& center() const { return _center; }
	float radius() const { return _radius; }

protected:
	std::pair<float, float> getUV(const Vec3& normal) const;

	Point _center;
	float _radius;
};

class MovingSphere : public Sphere
{
public:
	explicit MovingSphere(const Point& center, float radius, const std::shared_ptr<Material>& material, const Vec3& velocity, float time = 0.0f)
		: Sphere(center, radius, material), _velocity(velocity) {}

	bool hit(const Ray& ray, float min, float max, IntersectInfo& info) const override;

	Point center(float time) const { return _center + _velocity * time; }

private:
	Vec3 _velocity;
};

class RectangleXY : public RenderObject
{
public:
	explicit RectangleXY(float x0, float x1, float y0, float y1, float z, const std::shared_ptr<Material>& material)
		: x0(x0), x1(x1), y0(y0), y1(y1), z(z), RenderObject(material) {}

	bool hit(const Ray& ray, float min, float max, IntersectInfo& info) const override;
	AABB createBox() const override;

private:
	float x0, x1, y0, y1, z;
};

class RectangleYZ : public RenderObject
{
public:
	explicit RectangleYZ(float y0, float y1, float z0, float z1, float x, const std::shared_ptr<Material>& material)
		: y0(y0), y1(y1), z0(z0), z1(z1), x(x), RenderObject(material) {}

	bool hit(const Ray& ray, float min, float max, IntersectInfo& info) const override;
	AABB createBox() const override;

private:
	float y0, y1, z0, z1, x;
};

class RectangleXZ : public RenderObject
{
public:
	explicit RectangleXZ(float x0, float x1, float z0, float z1, float y, const std::shared_ptr<Material>& material)
		: x0(x0), x1(x1), z0(z0), z1(z1), y(y), RenderObject(material) {}

	bool hit(const Ray& ray, float min, float max, IntersectInfo& info) const override;
	AABB createBox() const override;

private:
	float x0, x1, z0, z1, y;
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

	bool hit(const Ray& ray, float min, float max, IntersectInfo& info, std::shared_ptr<RenderObject>& object) const;
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

	bool hit(const Ray& ray, float min, float max, IntersectInfo& info, std::shared_ptr<RenderObject>& object) const;
	void buildBVHTree() { _tree = BVHTree(_objects); }

private:
	BVHTree _tree;
	std::vector<std::shared_ptr<RenderObject>> _objects;
};

class Cuboid : public RenderObject
{
public:
	explicit Cuboid(const Point& p1, const Point& p2, const std::shared_ptr<Material>& material);

	bool hit(const Ray& ray, float min, float max, IntersectInfo& info) const override;
	AABB createBox() const override;

private:
	Point _pmin;
	Point _pmax;
	Scene scene;
};

class Rotate_Y : public RenderObject
{
public:
	explicit Rotate_Y(const std::shared_ptr<RenderObject>& object, float angle) : _object(object), _angle(angle), RenderObject(object->material()) {}

	bool hit(const Ray& ray, float min, float max, IntersectInfo& info) const override;
	AABB createBox() const override;

private:
	std::shared_ptr<RenderObject> _object;
	float _angle;
};

#endif