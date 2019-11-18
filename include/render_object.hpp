
#include "vec3f.hpp"
#include "intersection.hpp"
#include "ray.hpp"

#pragma once
namespace geometry {

	class RenderObject {
	public:
		RenderObject() = default;
		RenderObject(math::Vec3f color) : color(color) {};
		RenderObject(math::Vec3f color, bool reflects) : color(color), reflects(reflects) {};
		//destructor
		virtual ~RenderObject() {}

		//intersection function, returns maybe type
		virtual Intersection intersection(Ray &ray) const = 0;

		math::Vec3f color = { .3f,0.f,0.f };
		bool reflects = false;

	};
}