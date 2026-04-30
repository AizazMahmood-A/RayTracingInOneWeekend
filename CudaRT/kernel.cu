#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <curand_kernel.h>
#include "rtweekend.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "camera.h"
#include "sphere.h"
#include "hittable_list.h"

#define checkCudaErrors(val) check_cuda((val), #val, __FILE__, __LINE__)
void check_cuda(cudaError_t result, char const* const func, const char* const file, int const line) {
	if (result) {
		std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
			file << ":" << line << " '" << func << "' \n";

		// Make sure we call CUDA Device Reset before exiting.
		cudaDeviceReset();
		exit(99);
	}
}

//__global__ void rand_init(curandState* rand_state) {
//	if (threadIdx.x == 0 && blockIdx.x == 0) {
//		curand_init(1984, 0, 0, rand_state);
//	}
//}
//
//__global__ void render_init(int max_x, int max_y, curandState* rand_state) {
//	int i = threadIdx.x + blockIdx.x * blockDim.x;
//	int j = threadIdx.y + blockIdx.y * blockDim.y;
//
//	if ((i >= max_x) || (j >= max_y)) return;
//	int pixel_index = j * max_x + i;
//
//	curand_init(1984 + pixel_index, 0, 0, &rand_state[pixel_index]);
//}

__global__ void init_random_kernel(curandState* state, unsigned long seed, int width, int height) {
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;

	if ((x >= width) || (y >= height)) return;
	int id = y * width + x;

	// Each thread gets a different seed, or the same seed with different sequence numbers
	curand_init(seed, id, 0, &state[id]);

}

__global__ void setup_world(camera** cam, hittable** world, hittable_list** world_list, int width, int height) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {

		*cam = new camera();

		(**cam).aspect_ratio = 16.0 / 9.0;
		(**cam).image_width = 1200;
		(**cam).samples_per_pixel = 100;
		(**cam).max_depth = 50;
		(**cam).background = color(.70, .80, 1.00);

		(**cam).vfov = 20;
		(**cam).lookfrom = point3(13, 2, 3);
		(**cam).lookat = point3(0, 0, 0);
		(**cam).vup = vec3(0, 1, 0);

		(**cam).defocus_angle = 0.6;
		(**cam).focus_dist = 10.0;
		(**cam).Initialize();

		world[0] = new sphere(vec3(0, -1000.0, -1), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
		world[1] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
		world[2] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
		world[3] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

		*world_list = new hittable_list(world, 4);
	}
}

__global__ void render(curandState* state, vec3* frame_buffer, int max_x, int max_y, camera** cam, hittable** world, hittable_list** world_list) {
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	if ((i >= max_x) || (j >= max_y)) return;
	int pixel_index = j * max_x + i;
	int id = j * max_x + i;

	// copy state to local
	curandState localState = state[id];

	// generate a double [0.0, 1.0]
	//double val = curand_uniform_double(&localState);

	color pixel_color = (*cam)->render(state, (**world_list), i, j);
	frame_buffer[pixel_index] = pixel_color;//vec3(double(i) / max_x, double(j) / max_y, 0.2);

	state[id] = localState;
}

int main() {
	int width = 1200;
	int height = 600;

	int tx = 8;
	int ty = 8;

	int num_pixels = width * height;

	// allocate frame buffer
	size_t frame_buffer_size = num_pixels * sizeof(vec3);
	vec3* frame_buffer;
	checkCudaErrors(cudaMallocManaged((void**)&frame_buffer, frame_buffer_size));

	// allocate curand state
	size_t state_size = num_pixels * sizeof(curandState);
	curandState* curand_state;
	checkCudaErrors(cudaMalloc(&curand_state, state_size));

	dim3 blocks(width / tx + 1, height / ty + 1);
	dim3 threads(tx, ty);

	// init random state
	init_random_kernel << <blocks, threads >> > (curand_state, time(NULL), width, height);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	// init world objects
	camera** cam;
	checkCudaErrors(cudaMalloc((void**)&cam, sizeof(cam)));

	hittable** world;
	int num_objects = 4;
	checkCudaErrors(cudaMalloc((void**)&world, num_objects * sizeof(world)));

	hittable_list** world_list;
	checkCudaErrors(cudaMalloc((void**)&world_list, sizeof(world_list)));

	setup_world << <blocks, threads >> > (cam, world, world_list, width, height);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	// render
	render << <blocks, threads >> > (curand_state, frame_buffer, width, height, cam, world, world_list);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	// output frame buffer as image
	std::cout << "P3\n" << width << " " << height << "\n255\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			size_t pixel_index = j * width + i;

			write_color(std::cout, frame_buffer[pixel_index]);
			//double r = frame_buffer[pixel_index].x();
			//double g = frame_buffer[pixel_index].y();
			//double b = frame_buffer[pixel_index].z();

			//int ir = int(/*255.99 **/ r);
			//int ig = int(/*255.99 **/ g);
			//int ib = int(/*255.99 **/ b);
			//std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	checkCudaErrors(cudaFree(frame_buffer));
	checkCudaErrors(cudaFree(curand_state));
	checkCudaErrors(cudaFree(cam));
	checkCudaErrors(cudaFree(world));
}