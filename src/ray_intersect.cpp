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

#include "ray_intersect.hpp"

namespace geometry {

Hit::operator bool() const { return didIntersect; }

Hit intersect(Ray ray, Triangle const &triangle) {
  Hit hit;
  // TODO
  return hit;
}

Hit intersect(Ray const &ray, Sphere const &sphere) {
  Hit hit;
  //TODO
  return hit;
}

Hit intersect(Ray const &r, Plane const &p) {
  Hit hit;

  auto denom = r.direction * p.normal;
  if (std::abs(denom) < 1e-5f)
    return hit;

  auto t = ((p.origin - r.origin) * p.normal) / denom;

  if (t < 0.f)
    return hit;

  hit.didIntersect = true;
  hit.rayDepth = t;

  return hit;
}

} // namespace geometry
