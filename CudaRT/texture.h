#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "rtw_stb_image.h"

class texture {
public:
	__device__ virtual ~texture() = default;
	__device__ virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
	__device__ solid_color(const color& albedo) : albedo(albedo) {}
	__device__ solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}
	__device__ color value(double u, double v, const point3& p) const override {
		return color(0.4, 0.2, 0.1);//albedo;
	}
private:
	color albedo;
};

//class checker_texture : public texture {
//public:
//	__device__ checker_texture(double scale, texture* even, texture* odd)
//		: inv_scale(1.0 / scale), even(even), odd(odd) {
//	}
//
//	__device__ checker_texture(double scale, const color& c1, const color& c2)
//		: checker_texture(scale, &solid_color(c1), &solid_color(c2)) {
//	}
//
//	__device__ color value(double u, double v, const point3& p) const override {
//		auto xInteger = int(floor(inv_scale * p.x()));
//		auto yInteger = int(floor(inv_scale * p.y()));
//		auto zInteger = int(floor(inv_scale * p.z()));
//
//		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
//		return isEven ? even->value(u, v, p) : odd->value(u, v, p);
//	}
//
//
//private:
//	double inv_scale;
//	texture* even;
//	texture* odd;
//};

//class image_texture : public texture {
//public:
//	image_texture(const char* filename) : image(filename) {}
//
//	color value(double u, double v, const point3& p) const override {
//		// If we have no texture data, then return solid cyan as a debugging aid.
//		if (image.height() <= 0) return color(0, 1, 1);
//
//		// Clamp input texture coordinates to [0,1] x [1,0]
//		u = interval(0, 1).clamp(u);
//		v = 1.0 - interval(0, 1).clamp(v); // Flip V to image coordinates
//
//		auto i = int(u * image.width());
//		auto j = int(v * image.height());
//		auto pixel = image.pixel_data(i, j);
//		auto color_scale = 1.0 / 255.0;
//		return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
//	}
//private:
//	rtw_image image;
//};

//class noise_texture : public texture {
//public:
//	noise_texture(double scale) : scale(scale) {}
//
//	color value(double u, double v, const point3& p) const override {
//		return color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7))); //noise.turb(p, 7);
//	}
//
//private:
//	perlin noise;
//	double scale;
//};

#endif // !TEXTURE_H
