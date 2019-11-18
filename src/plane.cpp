/**
 * @author	Andrew Robert Owens, Philmo Gu
 * @date 	2019-10-15.
 * @details Organization: Biological Modeling and Visualization research group
 * University of Calgary, Calgary, AB, Canada
 *
 * Contact:	arowens [at] ucalgary.ca
 * @copyright Copyright (c) 2019 ___ALGORITHMIC_BOTANY___. All rights reserved.
 *
 * @brief
 */

#include <cstdlib> 
#include "plane.hpp"

namespace geometry {

	Plane::Plane(math::Vec3f normal, math::Vec3f origin) : normal(normal), origin(origin) {}

	Plane::Plane(math::Vec3f normal, math::Vec3f origin, math::Vec3f color) : normal(normal), origin(origin), RenderObject(color) {}
	Plane::Plane(math::Vec3f normal, math::Vec3f origin, math::Vec3f color, bool reflects) : normal(normal), origin(origin), RenderObject(color,reflects) {}

	Intersection Plane::intersection(Ray& ray) const
	{
		Intersection hit;

		auto denom = ray.direction * normal;
		if (std::abs(denom) < 1e-5f)
			return Intersection::False();

		auto t = ((origin - ray.origin) * normal) / denom;

		//made slightly above zero to remove artifacts caused by floating point errors.
		if (t <= 0.001f)
			return Intersection::False();

		hit.exists = true;
		hit.rayDepth = t;
		hit.location = ray.origin + ray.direction * t;
		hit.normal = normal;
		hit.color = color;
		hit.continues = reflects;
		return hit;
	}
}