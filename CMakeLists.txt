cmake_minimum_required (VERSION 3.2)

set(BUILD_TARGET txbase)
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


file(GLOB_RECURSE BUILD_SOURCE "txbase/*.cc" "txbase/*.h")
add_library(${BUILD_TARGET} STATIC ${BUILD_SOURCE})

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

########## TESTING

# googletest
add_subdirectory(txbase_tests/libs/gtest)

enable_testing()
set(TEST_TARGET ${BUILD_TARGET}_tests)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})