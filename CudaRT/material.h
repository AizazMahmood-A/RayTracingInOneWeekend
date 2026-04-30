#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material
{
public:
	//__device__ virtual ~material() = default;

	__device__ virtual color emitted(double u, double v, const point3& p) const {
		return color(0, 0, 0);
	}

	__device__ virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, curandState* state
	) const {
		return false;
	}
};

class lambertian : public material {
public:
	__device__ lambertian(const color& albedo) : tex(albedo) {}
	//__device__ lambertian(texture* tex) : tex(tex) {}

	__device__ bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, curandState* state)
		const override {
		auto scatter_direction = rec.normal + random_unit_vector(state);

		// Catch degenerate scatter direction
		if (scatter_direction.near_zero()) {
			scatter_direction = rec.normal;
		}

		scattered = ray(rec.p, scatter_direction, r_in.time());
		attenuation = tex;
		return true;
	}

private:
	color tex;
};

class metal : public material {
public:
	__device__ metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	__device__ bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, curandState* state)
		const override {
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected + (fuzz * random_unit_vector(state)));

		scattered = ray(rec.p, reflected, r_in.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
private:
	color albedo;
	double fuzz;
};

class dielectric : public material {
public:
	__device__ dielectric(double refraction_index) : refraction_index(refraction_index) {}

	__device__ bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, curandState* state)
		const override {
		attenuation = color(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, ri) > random_double(state))
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, ri);

		scattered = ray(rec.p, direction, r_in.time());
		return true;
	}

private:
	// Refractive index in vacuum or air or the ratio of material's refractive index over
	// the refractive index of the enclosing media.
	double refraction_index;

	__device__ static double reflectance(double cosine, double refraction_index) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

//class diffuse_light : public material {
//public:
//	diffuse_light(shared_ptr<texture> tex) : tex(tex) {}
//	diffuse_light(const color& emit) : tex(make_shared<solid_color>(emit)) {}
//
//	color emitted(double u, double v, const point3& p) const override {
//		return tex->value(u, v, p);
//	}
//
//private:
//	shared_ptr<texture> tex;
//};
//
//class isotropic : public material {
//public:
//	isotropic(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
//	isotropic(shared_ptr<texture> tex) : tex(tex) {}
//
//	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
//		const override {
//
//		scattered = ray(rec.p, random_unit_vector(), r_in.time());
//		attenuation = tex->value(rec.u, rec.v, rec.p);
//		return true;
//	}
//
//private:
//	shared_ptr<texture> tex;
//};

#endif // !MATERIAL_H
