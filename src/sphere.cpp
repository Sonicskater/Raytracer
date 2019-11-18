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

#include "sphere.hpp"
#include "ray.hpp"
#include <memory>

namespace geometry {


	bool quadraticFormula(const float &a, const float &b, const float &c, float& x0, float& x1) {

		// https://stackoverflow.com/questions/48979861/numerically-stable-method-for-solving-quadratic-equations
		// more stable version of quadratic formula to prevent errors due to bad precison on computers

		float root = (b * b) - (4 * a * c);
		if (root < 0) {
			return false;
		}
		else if (root == 0) {
			x0 = x1 = -0.5 * (b / a);
		}
		else {

			float q;
			if (b > 0) {
				q = -0.5 * (b + sqrt(root));
			}
			else {
				q = -0.5 * (b - sqrt(root));
			}
			x0 = q / a;
			x1 = c / q;
		}

		// always return the lowest soloution first.
		if (x0 > x1) std::swap(x0, x1);

		return true;
	}

	Intersection Sphere::intersection(Ray& ray) const
	{
		Intersection hit;


		//https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection

		//soloutions 
		float t0, t1;

		

		//direction of line from sphere to ray origin
		math::Vec3f L = ray.origin - origin();

		//quadratic setup
		float a = ray.direction * ray.direction;
		float b = 2 * ray.direction * L;
		float c = L * L - (radius() * radius());

		if (!quadraticFormula(a, b, c, t0, t1)) {
			return Intersection::False();
		}
		if (t0 > t1) std::swap(t0, t1);

		// uses 0.01 for floating point artifact mitigation
		// <= so that shadow rays dont collide with their soruces.
		if (t0 <= 0.01) {
			// t0 is behind, check t1;
			t0 = t1;
			if (t0 <= t1) {
				return Intersection::False();
				// both behind
			}
		}

		hit.exists = true;
		hit.color = color;
		hit.rayDepth = t0;
		// use distance to find actual intersection point
		hit.location = ray.origin + ray.direction * t0;
		//get normal by tracknig line through the intersection point from the origin of the sphere
		hit.normal = (hit.location - origin()).normalize();
		hit.continues = reflects;
		return hit;
	}
}