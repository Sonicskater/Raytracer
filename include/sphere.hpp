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

#pragma once

#include "vec3f.hpp"
#include "intersection.hpp"
#include "render_object.hpp"


namespace geometry {


class Sphere : public RenderObject{
public:
  Sphere() = default;
  Sphere(math::Vec3f origin, float radius) : m_origin(origin), m_radius(radius) {};
  Sphere(math::Vec3f origin, float radius, math::Vec3f color) : m_origin(origin), m_radius(radius), RenderObject(color) {};
  Sphere(math::Vec3f origin, float radius, math::Vec3f color, bool reflective) : m_origin(origin), m_radius(radius), RenderObject(color,reflective) {};

  float radius() const { return m_radius; }
  math::Vec3f origin() const { return m_origin; }

  float& radius() { return m_radius; }
  math::Vec3f& origin() { return m_origin; }

private:
	math::Vec3f m_origin;
	float m_radius = 1.f;

	// Inherited via RenderObject
	virtual Intersection intersection(Ray& ray) const override;
};

} // namespace geometry