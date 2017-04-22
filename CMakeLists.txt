cmake_minimum_required (VERSION 3.2)

project(txbase)

#vscode-cmake-tools target debugging
include(CMakeToolsHelpers OPTIONAL)

# set(CMAKE_VERBOSE_MAKEFILE 1)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(CMAKE_CXX_STANDARD 14)

if(MSVC)
	add_definitions(/arch:SSE)
	add_definitions(/arch:SSE2)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse3")
endif()
# http://stackoverflow.com/questions/2368811/how-to-set-warning-level-in-cmake
# if(MSVC)
#   # Force to always compile with W4
#   if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
#     string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#   else()
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
#   endif()
# elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#   # Update if necessary
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# endif()

set(BUILD_TARGET txbase)

#file(GLOB_RECURSE all_files txbase/*)
#message(STATUS "${all_files}")

add_library(${BUILD_TARGET} STATIC
	txbase/gui/drawing.cc
	txbase/gui/drawing.h
	txbase/gui/fontmap.cc
	txbase/gui/fontmap.h
	txbase/gui/fonts/droid_sans.h
	txbase/gui/gui.cc
	txbase/gui/gui.h
	txbase/image/film.cc
	txbase/image/film.h
	txbase/image/filter.h
	txbase/image/image.cc
	txbase/image/image.h
	txbase/libs/sse_mathfun.h
	txbase/libs/stb_image.h
	txbase/libs/stb_image_write.h
	txbase/libs/stb_truetype.h
	txbase/libs/tiny_obj_loader.cc
	txbase/libs/tiny_obj_loader.h
	txbase/math/base.h
	txbase/math/bbox.h
	txbase/math/color.cc
	txbase/math/color.h
	txbase/math/geometry.cc
	txbase/math/geometry.h
	txbase/math/matrix.cc
	txbase/math/matrix.h
	txbase/math/quaternion.cc
	txbase/math/quaternion.h
	txbase/math/random.h
	txbase/math/ray.cc
	txbase/math/ray.h
	txbase/math/sample.cc
	txbase/math/sample.h
	txbase/math/transform.cc
	txbase/math/transform.h
	txbase/math/vector.h
	txbase/misc/const.h
	txbase/misc/randomdata.cc
	txbase/misc/randomdata.h
	txbase/opengl/application.cc
	txbase/opengl/application.h
	txbase/opengl/input.h
	txbase/opengl/opengl.cc
	txbase/opengl/opengl.h
	txbase/scene/camera.cc
	txbase/scene/camera.h
	txbase/scene/camera_controller.h
	txbase/shader/base.h
	txbase/shader/default.vs.glsl
	txbase/shader/phong.h
	txbase/shader/phong/phong.cc
	txbase/shader/phong/phong.fs.glsl
	txbase/shader/shaderprogram.cc
	txbase/shader/singlecolor.fs.glsl
	txbase/shape/mesh.cc
	txbase/shape/mesh.h
	txbase/shape/obj.cc
	txbase/shape/obj.h
	txbase/shape/shape.h
	txbase/sse/bool.h
	txbase/sse/float.cc
	txbase/sse/float.h
	txbase/sse/int.cc
	txbase/sse/int.h
	txbase/sse/sse.h
	txbase/sys/memory.h
	txbase/sys/thread.cc
	txbase/sys/thread.h
	txbase/sys/tools.cc
	txbase/sys/tools.h

	txbase/fwddecl.h
	txbase/stdafx.cc
	txbase/stdafx.h
)

target_include_directories(${BUILD_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

########## DEPENDENCIES

# gl3w: dependency of imgui and the app
add_subdirectory(txbase/libs/gl3w)

# glfw3
find_package(glfw3 REQUIRED)
target_include_directories(${BUILD_TARGET} PUBLIC ${GLFW3_INCLUDE_DIRS})

# opengl
find_package(OpenGL REQUIRED)
target_include_directories(${BUILD_TARGET} PUBLIC ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${BUILD_TARGET}
	${OPENGL_LIBRARIES}
	gl3w
	${GLFW3_LIBRARIES}
)
