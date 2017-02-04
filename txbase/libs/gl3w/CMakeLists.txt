cmake_minimum_required (VERSION 3.2)

set(BUILD_TARGET gl3w)
add_library(${BUILD_TARGET} STATIC
	GL/gl3w.c
	GL/gl3w.h
	GL/glcorearb.h
)

# opengl
find_package(OpenGL REQUIRED)

target_include_directories(${BUILD_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${BUILD_TARGET} PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${BUILD_TARGET}
	${OPENGL_LIBRARIES}
	${CMAKE_DL_LIBS}
)
