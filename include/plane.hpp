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
#include "render_object.hpp"

namespace geometry {

class Plane : public RenderObject {
public:
  Plane() = default;
  Plane(math::Vec3f normal, math::Vec3f origin);
  Plane(math::Vec3f normal, math::Vec3f origin, math::Vec3f color);
  Plane(math::Vec3f normal, math::Vec3f origin, math::Vec3f color, bool reflects);
  math::Vec3f normal = {0.f, 1.f, 0.f};
  math::Vec3f origin = {0.f, 0.f, 0.f};

  // Inherited via RenderObject
  virtual Intersection intersection(Ray& ray) const override;
};

} // namespace geometry
