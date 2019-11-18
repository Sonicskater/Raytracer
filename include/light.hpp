
#include "vec3f.hpp"

class Light {
public:
	Light() = default;
	Light(math::Vec3f location) : location(location) {};
	Light(math::Vec3f location, math::Vec3f intensity) : location(location), intensity(intensity) {};
	math::Vec3f location = {0.f,0.f,0.f};
	math::Vec3f intensity = { 1.f,1.f,1.f };
};