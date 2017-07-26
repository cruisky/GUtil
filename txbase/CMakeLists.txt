# cmake_minimum_required (VERSION 3.2)

set(BUILD_TARGET txbase)

file(GLOB_RECURSE BUILD_SOURCE "./*.cc" "./*.h")
add_library(${BUILD_TARGET} STATIC ${BUILD_SOURCE})

target_include_directories(${BUILD_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

########## DEPENDENCIES

# gl3w: dependency of imgui and the app
add_subdirectory(libs/gl3w)

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
