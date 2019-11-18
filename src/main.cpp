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

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert> //assert
#include <random>

#include "mat3f.hpp"

#include "vec3f.hpp"
#include "vec2f.hpp"
#include "image.hpp"
#include "vec2i.hpp"
#include "grid2.hpp"
#include "timer.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include <memory>
#include "light.hpp"
#include "render_triangle.hpp"
// only do this kinda stuff in main.cpp or *.cpp; never *.h
using namespace math;
using namespace geometry;
using namespace std;

namespace math {
	float distance(Vec2f const& a, Vec2f const& b) { return norm(b - a); }
}

namespace raytracing {

Vec3f normalAt(Vec3f const &p, Sphere const &s) {
  Vec3f n;
  return n;
}

Vec3f normalAt(Vec3f const & /*p*/, Plane const &p) {
  return p.normal;
}


struct ImagePlane {
  using Screen = geometry::Grid2<raster::RGB>;

  Screen screen;
  math::Vec3f origin;
  math::Vec3f u;
  math::Vec3f v;
  float left;   // right symmetric
  float bottom; // top symmetric

  ImagePlane &resolution(uint32_t width, uint32_t height) {
    screen = Screen(width, height);
    return *this;
  }
  ImagePlane &center(Vec3f center) {
    origin = center;
    return *this;
  }
  ImagePlane &uvAxes(Vec3f up, Vec3f right) {
    u = up;
    v = right;
    return *this;
  }
  ImagePlane &dimensions(float width, float height) {
    left = -(0.5f * width);
    bottom = -(0.5f * height);
    return *this;
  }

  math::Vec3f pixelTo3D(math::Vec2f pixel) const {
    using std::abs;

    // shift to center
    pixel += {0.5f, 0.5f};

    float u_x = left + (2.f * abs(left)) * (pixel.x) / screen.width();
    float v_y = bottom + (2.f * abs(bottom)) * (pixel.y) / screen.height();

    return origin + u_x * u + v_y * v;
  }
};

ImagePlane makeImagePlane(Vec3f const &eye, Vec3f const &lookatPosition,
                          Vec3f const &canonicalUp, int xResolution,
                          int yResolution, float width, float height,
                          float nearPlaneDistanace) {
  // make orthonormal basis around eye
  auto gaze = normalized(lookatPosition - eye);

  auto u = gaze ^ canonicalUp;
  u.normalize();

  auto v = u ^ gaze;
  v.normalize();

  // image plane is orthogoanl to camera gaze so use u,v for image plane
  ImagePlane imagePlane;
  // using method chaining to have named parameter/configuation)
  imagePlane.resolution(xResolution, yResolution)
      .center(eye + gaze * nearPlaneDistanace)
      .dimensions(width, height)
      .uvAxes(u, v);

  return imagePlane;
}


Vec3f Reflect(Vec3f &n, Vec3f &i) {
	return i - 2 * (i * n) * n;
}

Vec3f castRay(Ray ray,
	math::Vec3f eye,                    //
	std::vector<Light> const& lights,//
	std::vector<std::unique_ptr<RenderObject>> const& objects) {

	constexpr float ambientIntensity = 0.15f;

	// background color
	Vec3f colorOut(0.1f, 0.1f, 0.1f);
	Vec3f nextColor(0.f, 0.f, 0.f);
	Vec3f selfColor(0.f, 0.f, 0.f);

	// find closed object, if any
	Intersection closest;
	Vec3f lightDir;
	// pointer to closest object
	for (auto const& s : objects) {
		auto hit = s->intersection(ray);
		if (hit && (hit.rayDepth < closest.rayDepth)) {
			
			closest = hit;
		}
	}

	// if hit get point
	if (closest) {
		//phong shading

		float diffuse = 0, specular = 0;
		for (auto const& l : lights) {
			lightDir = (l.location - closest.location).normalize();

			Ray shadowRay = Ray(closest.location, lightDir);
			Intersection hit;
			for (auto const& s : objects) {
				hit = s->intersection(shadowRay);
				if (hit) {
					break;
				}
			}
			if (!hit) {

				diffuse += 0.3 * std::max(0.f, closest.normal * lightDir);

				Vec3f incident = -1 * lightDir;
				Vec3f reflected = Reflect(closest.normal, incident);

				specular += 0.5 * std::pow(std::max(0.f, reflected * (-1 * ray.direction)), 4);
			}
		}
		selfColor = closest.color * (diffuse + ambientIntensity) + Vec3f(1, 1, 1) * specular;
		if (closest.continues) {

			nextColor = castRay(Ray(closest.location, Reflect(closest.normal, ray.direction)), eye, lights, objects);
			
			//controls contribution of reflected light to final color. Higher = more reflective, must be less than 0 <x <1
			float reflective_mix = 0.75;
			colorOut = nextColor*reflective_mix + selfColor * (1-reflective_mix);
		}
		else {
			colorOut = selfColor;
		}

		
	}
	return colorOut;
}

void render(ImagePlane& imagePlane, //
	math::Vec3f eye,        // all below could be in 'scene' object
	std::vector<Light> const& lights,     //
	std::vector<std::unique_ptr<RenderObject>> const& surfaces) {

	// Standard mersenne_twister_engine seeded
	thread_local std::mt19937 gen(0);
	auto sampleRange = [](float a, float b) {
		using distrubution = std::uniform_real_distribution<>;
		return distrubution(a, b)(gen);
	};

	// question? if I switch the order here of rows vs. coloumns is it faster?
	// slower? why?
	for (int32_t y = 0; y < imagePlane.screen.height(); ++y) {
		for (int32_t x = 0; x < imagePlane.screen.width(); ++x) {

			math::Vec2f pixel(x, y);
			auto pixel3D = imagePlane.pixelTo3D(pixel);
			auto direction = normalized(pixel3D - eye);
			auto bias = 1e-4f;
			auto p = pointOnLne(eye, direction, bias);
			Ray r(p, direction);

			auto colorOut = castRay(r, eye, lights, surfaces);

			// correct to quantiezed error
			// (i.e., removes banded aliasing when converting to 8bit RGB)
			constexpr float halfStep = 1.f / 512;
			colorOut = raster::quantizedErrorCorrection(
				colorOut, sampleRange(-halfStep, halfStep));

			imagePlane.screen({ x, y }) = raster::convertToRGB(colorOut);
		}
	}
}
} // namespace

int main(int argc, char** argv) {

  // setup camera and imagePlane
  Vec3f light{20, 15, 10};

  // perspectives
  //Vec3f eye{20.f, 0.f,-30.f};
  //Vec3f eye{ 0.f,10.f,10.f };
  //Vec3f eye{ 0.f,10.f,-30.f };
  //Vec3f eye{ 10.f,0.f,0.f };
  //Vec3f eye{ 30.f,30.f,0.f };
  Vec3f eye{ 0.f,0.f,10.f };

  Vec3f lookat{0.f, 0.f, 0.f};
  //Vec3f lookat{0.f, -4.8f, 0.f};
  Vec3f up{0.f, 1.f, 0.f};
  int resolutionX = 1000;
  int resolutionY = 1000;
  float planeWidth = 50.f;
  float planeHeight = 50.f;
  float focalDist = 50.f;
  if (argc == 3) {
	  resolutionX = stoi(argv[1]);
	  resolutionY = stoi(argv[2]);
	  if (resolutionY < resolutionX) {

		  planeWidth = planeHeight * (resolutionX / resolutionY);
	  }
	  else {

		  planeHeight = planeWidth * (resolutionY / resolutionX);
	  }
  }
  std::vector<Light> lights = vector<Light>();

  using namespace raytracing;
  using namespace std;
  auto imagePlane = makeImagePlane(eye, lookat, up,          //
                                   resolutionX, resolutionY, //
                                   planeWidth, planeHeight,  //
                                   focalDist);
  
  lights.push_back(Light(light));
  //lights.push_back(Light({-20,15,10}));
  lights.push_back(Light({ 0,15,0 }));
  // setup scene
  vector<unique_ptr<RenderObject>> objects;
  Plane p;
  //surfaces.push_back(makeIntersectable(p));
  //auto f = std::unique_ptr<RenderObject>(new Plane (std::move(p)));
  auto f = unique_ptr<RenderObject>(new Sphere({ -1.f,0.f,0.f }, 1.f));
  objects.push_back(move(f));

  f = unique_ptr<RenderObject>(new Sphere({ 2.f,3.f,-2.f }, 1.f, { .3f,.3f,0.f }, true));
  objects.push_back(move(f));

  f = unique_ptr<RenderObject>(new RenderTriangle({ 0,9,-13 }, { 5,5,7 }, { 5,0,-13 },  { 0, 0.5f, 0 }, true));
  objects.push_back(move(f));

  f = unique_ptr<RenderObject>(new Sphere({ 6.f, 0.f, -5.f }, 2.f, {0.f,.3f,0.f}, true));
  objects.push_back(move(f));

  f = unique_ptr<RenderObject>(new Sphere({ -2.f, 1.f, -9.f }, 5.f, { 0.f,.3f,.3f }, true));
  objects.push_back(move(f));

  f = unique_ptr<RenderObject>(new Sphere({ -2.f, -3.8f, -13.f }, 1.f, { 1.f,1.f,0.f }, true));
  objects.push_back(move(f));

  f = unique_ptr<RenderObject>(new Plane({ 0.f,1.f,0.f }, { 0.f, -5.f, 0.f }, {.3f,0.f,.3f}, true));
  objects.push_back(move(f));

  //Sphere s = Sphere({ 0.f,0.f,0.f }, 1.f);
  //surfaces.push_back(makeIntersectable(s));
  //surfaces.push_back(makeIntersectable(new RedSphere({ 0.f, 0.f, 0.f }, 1.f)));
  //surfaces.push_back(makeIntersectable(new BlueSphere({ 1.f, 1.f, -0.5f }, 1.f)));
  //surfaces.push_back(makeIntersectable(new RedSphere({ 1.f, 0.f, -1.f }, 2.f)));
  //surfaces.push_back(makeIntersectable(new BlueSphere({ -2.f, 0.f, -2.f }, 5.f)));
  // render that thing...
  temporal::Timer timer(true);

  //renderOld(imagePlane, eye, light, surfaces);
  render(imagePlane, eye, lights, objects);

  std::cout << "Time elapsed: " << std::fixed << timer.minutes() << " min.\n";

  raster::write_screen_to_file("./test.png", imagePlane.screen);

  return EXIT_SUCCESS;
}
