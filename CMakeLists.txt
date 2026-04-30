# CMakeList.txt : CMake project for RayTracingInOneWeekend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.18...4.0.0)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Add CUDA and CXX to Project
project ("RayTracingInOneWeekend" LANGUAGES CXX CUDA)

# add CUDAToolKit
find_package(CUDAToolkit REQUIRED)

# Set CXX Standard
set ( CMAKE_CXX_STANDARD 20 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON)
set ( CMAKE_CXX_EXTENSIONS OFF)

# Set CUDA Standard
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_ARCHITECTURES "native")

# Source
set( EXTERNAL 
"external/stb_image.h")

set( SOURCE_ONE_WEEKEND 
"OneWeekend/RayTracingInOneWeekend.h"
"OneWeekend/RayTracingInOneWeekend.cpp"
"OneWeekend/vec3.h"
"OneWeekend/color.h" 
"OneWeekend/ray.h" 
"OneWeekend/hittable.h" 
"OneWeekend/sphere.h" 
"OneWeekend/hittable_list.h" 
"OneWeekend/rtweekend.h" 
"OneWeekend/interval.h" 
"OneWeekend/camera.h" 
"OneWeekend/material.h" )

set( SOURCE_NEXT_WEEK 
"TheNextWeek/RayTracingInOneWeekend.h"
"TheNextWeek/RayTracingInOneWeekend.cpp"
"TheNextWeek/vec3.h"
"TheNextWeek/color.h" 
"TheNextWeek/ray.h" 
"TheNextWeek/hittable.h" 
"TheNextWeek/sphere.h" 
"TheNextWeek/hittable_list.h" 
"TheNextWeek/rtweekend.h" 
"TheNextWeek/interval.h" 
"TheNextWeek/camera.h" 
"TheNextWeek/material.h" 
"TheNextWeek/aabb.h" 
"TheNextWeek/bvh.h" 
"TheNextWeek/texture.h" 
"TheNextWeek/rtw_stb_image.h" 
"TheNextWeek/perlin.h"
"TheNextWeek/quad.h" 
"TheNextWeek/constant_medium.h" )

set( SOURCE_CUDA_RT 
"CudaRT/kernel.cu"
 "CudaRT/aabb.h" 
 "CudaRT/bvh.h" 
 "CudaRT/camera.h" 
 "CudaRT/color.h" 
 "CudaRT/constant_medium.h" 
 "CudaRT/hittable.h" 
 "CudaRT/hittable_list.h" 
 "CudaRT/interval.h" 
 "CudaRT/material.h" 
 "CudaRT/perlin.h" 
 "CudaRT/quad.h" 
 "CudaRT/ray.h" 
 "CudaRT/rtw_stb_image.h" 
 "CudaRT/rtweekend.h" 
 "CudaRT/sphere.h" 
 "CudaRT/texture.h" 
 "CudaRT/vec3.h")

# Specific compiler flags below. We're not going to add options for all possible compilers, but if
# you're new to CMake (like we are), the following may be a helpful example if you're using a
# different compiler or want to set different compiler options.

message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message (STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message (STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})

if (MSVC)
    set(MSVC_CXX_FLAGS "/W4" "/we4265" "/we5204" "/wd4100")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${MSVC_CXX_FLAGS}>")
    add_compile_options("$<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=/W4>")
    add_compile_options("$<$<COMPILE_LANGUAGE:CUDA>:--expt-relaxed-constexpr>")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(UNIX_CXX_FLAGS "-Wnon-virtual-dtor" "-Wreorder" "-Wunused-variable")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${UNIX_CXX_FLAGS}>")
endif()

# Add source to this project's executable.
add_executable (RayTracingInOneWeekend ${EXTERNAL} ${SOURCE_ONE_WEEKEND})
add_executable (RayTracingTheNextWeek ${EXTERNAL} ${SOURCE_NEXT_WEEK})
add_executable (CudaRayTracing ${EXTERNAL} ${SOURCE_CUDA_RT})

# link CUDA libraries 
target_link_libraries(CudaRayTracing PRIVATE CUDA::cudart)
# target_link_libraries(CudaRayTracing PRIVATE CUDA::curand)

target_compile_definitions(CudaRayTracing PRIVATE USE_CUDA)
