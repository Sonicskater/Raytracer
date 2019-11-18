
#include "vec3f.hpp"
#include <limits>
#include <cmath>

#pragma once
class Intersection {
public:
	Intersection() : exists(false) {}

	using False = Intersection;
	// just so we can use it as a bool in if statements, syntactic-sugar
	explicit operator bool() const;

	bool exists;
	math::Vec3f normal;
	float rayDepth =  std::numeric_limits<float>::max();
	math::Vec3f location;
	math::Vec3f color;
	bool continues;
};