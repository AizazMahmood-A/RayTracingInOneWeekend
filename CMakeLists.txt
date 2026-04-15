# CMakeList.txt : CMake project for RayTracingInOneWeekend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8...4.0.0)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("RayTracingInOneWeekend")

set ( CMAKE_CXX_STANDARD 20 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON)
set ( CMAKE_CXX_EXTENSIONS OFF)

# Source

set(EXTERNAL 
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
"TheNextWeek/rtw_stb_image.h")


# Specific compiler flags below. We're not going to add options for all possible compilers, but if
# you're new to CMake (like we are), the following may be a helpful example if you're using a
# different compiler or want to set different compiler options.

message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message (STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message (STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # /wd #### - Disable warning
    # /we #### - treat warning as error
    add_compile_options("/W4")      # Enable level-4 warnings
    add_compile_options("/we 4265") # Class has virtual functions, but its non-trivial destructor is not virtual
    add_compile_options("/we 5204") # Class has virtual functions, but its trivial destructor is not virtual
    add_compile_options("/wd 4100") # unreferenced formal parameter
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wnon-virtual-dtor) # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder) # Data member will be initialized after [other] data member
    add_compile_options(-Wmaybe-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable) # Variable is defined but unused
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wnon-virtual-dtor) # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder) # Data member will be initialized after [other] data member
    add_compile_options(-Wsometimes-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable) # Variable is defined but unused
endif()

# Add source to this project's executable.
add_executable (RayTracingInOneWeekend ${EXTERNAL} ${SOURCE_ONE_WEEKEND})
add_executable (RayTracingTheNextWeek ${EXTERNAL} ${SOURCE_NEXT_WEEK})


