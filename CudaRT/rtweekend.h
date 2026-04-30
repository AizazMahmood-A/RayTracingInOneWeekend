#ifndef RTWEEKEND_H
#define RTWEEKEND_H

//#include <cmath>
//#include <cstdlib>
//#include <random>
#include <iostream>
#include <math_constants.h>
//#include <memory>

#include "device_launch_parameters.h"
#include <curand_kernel.h>

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

__host__ __device__ constexpr double infinity() { return std::numeric_limits<double>::infinity(); }
__host__ __device__ constexpr double pi() { return 3.1415926535897932385; }

// Utility Functions

__host__ __device__ inline double degrees_to_radians(double degrees) {
	return degrees * pi() / 180.0;
}

__device__ inline double random_double(curandState* state) {
	// Return a random real in [0, 1).
	//curand_uniform_double
	return curand(state) / (UINT_MAX + 1.0);
}

__device__ inline double random_double(curandState* state, double min, double max) {
	// Return a random real in [min, max).
	return min + (max - min) * random_double(state);
}

__device__ inline int random_int(curandState* state, int min, int max) {
	// Return a random integer in [min, max].
	return int(random_double(state, min, max + 1));
}

//__host__ inline double random_double() {
//	// Return a random real in [0, 1).
//	return std::rand() / (RAND_MAX + 1.0);
//}
//
//__host__ inline double random_double(double min, double max) {
//	// Return a random real in [min, max).
//	return min + (max - min) * random_double();
//}
//
//__host__ inline int random_int(int min, int max) {
//	// Return a random integer in [min, max].
//	return int(random_double(min, max + 1));
//}

//inline double random_double() {
//	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
//	static std::mt19937 generator;
//
//	return distribution(generator);
//}

// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif // !RTWEEKEND_H
