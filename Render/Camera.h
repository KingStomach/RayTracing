#ifndef _CARMERA_H_
#define _CARMERA_H_	

#include "../Array3/Point.h"

class Camera
{
public:
	Camera(const Point& position, const Vec3& front, const Vec3& up) : p(position), front(front), up(up) {}

	inline Point position() const { return p; }

private:
	Point p;
	Vec3 front;
	Vec3 up;
};


#endif