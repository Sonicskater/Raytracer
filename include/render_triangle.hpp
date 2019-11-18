
#include "plane.hpp"
#include "vec3f.hpp"
namespace geometry {
	class RenderTriangle : public Plane{
	public:
		RenderTriangle(math::Vec3f a, math::Vec3f b, math::Vec3f c) : Plane((b-a ^ c-a).normalize(), a) , a(a),b(b),c(c) {};
		RenderTriangle(math::Vec3f a, math::Vec3f b, math::Vec3f c, math::Vec3f color) : Plane((b - a ^ c - a).normalize(), a, color) , a(a), b(b), c(c) {};
		RenderTriangle(math::Vec3f a, math::Vec3f b, math::Vec3f c, math::Vec3f color, bool reflects) : Plane((b - a ^ c - a).normalize(), a, color, reflects) , a(a), b(b), c(c) {};
		math::Vec3f a, b, c;

		virtual Intersection intersection(Ray& ray) const override;
	};

}