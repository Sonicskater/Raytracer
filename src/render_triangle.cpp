#include "render_triangle.hpp"
#include "vec3f.hpp"
Intersection geometry::RenderTriangle::intersection(Ray& ray) const
{
	//call plane intersection to check if we should even bother.
	auto i = Plane::intersection(ray);
	if (i.exists) {
		math::Vec3f v;

		math::Vec3f e0 = b - a, vp0 = i.location - a;
		v = e0 ^ vp0;

		if (normal * v < 0) return Intersection::False();

		math::Vec3f e1 = c - b, vp1 = i.location - b;
		v = e1 ^ vp1;

		if (normal * v < 0) return Intersection::False();

		math::Vec3f e2 = a - c, vp2 = i.location - c;
		v = e2 ^ vp2;

		if (normal * v < 0) return Intersection::False();
	}
	else {
		return Intersection::False();
	}
	return i;
}
