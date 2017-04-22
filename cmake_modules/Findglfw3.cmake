IF(WIN32)
	FIND_PATH(GLFW3_INCLUDE_DIRS
		NAMES GLFW/glfw3.h
		PATHS $ENV{GLFW_ROOT}
		PATH_SUFFIXES include
		DOC "GLFW include directory"
		)
	FIND_LIBRARY(GLFW3_LIBRARIES
		NAMES glfw3
		PATHS $ENV{GLFW_ROOT}
		PATH_SUFFIXES lib lib-vc2015
		DOC "GLFW library directory"
		)
ELSE(WIN32)
	FIND_PATH(GLFW3_INCLUDE_DIRS
		NAMES GLFW/glfw3.h
		PATHS
			/usr/include
			/usr/local/include
		DOC "GLFW include directory"
		)
	FIND_LIBRARY(GLFW3_LIBRARIES
		NAMES libGLFW.a GLFW libGLFW3.a GLFW3 libglfw.so libglfw.so.3 libglfw.so.3.0
		PATHS
			/usr/lib64
			/usr/lib
			/usr/local/lib64
			/usr/local/lib
		DOC "GLFW library directory"
		)
ENDIF(WIN32)

message(STATUS "GLFW3 lib: ${GLFW3_LIBRARIES}, include: ${GLFW3_INCLUDE_DIRS}")

# handle the QUIETLY and REQUIRED arguments and set {Name}_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	glfw3 DEFAULT_MSG
	GLFW3_LIBRARIES
	GLFW3_INCLUDE_DIRS)


