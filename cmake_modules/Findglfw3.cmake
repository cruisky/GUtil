IF(WIN32)
	message(STATUS "Win32")
	FIND_PATH(GLFW3_INCLUDE_DIRS GLFW/glfw3.h
		$ENV{GLFW_ROOT}/include
		"c:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include"
		)
	FIND_LIBRARY(GLFW3_LIBRARY
		NAMES glfw3
		PATHS
		$ENV{GLFW_ROOT}/lib
		$ENV{GLFW_ROOT}/lib-vc2015
		"c:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/lib"
		)
ELSE(WIN32)
	FIND_PATH(GLFW3_INCLUDE_DIRS GLFW/glfw3.h
		/usr/include
		/usr/local/include
		)
	FIND_LIBRARY(GLFW3_LIBRARY
		NAMES libGLFW.a GLFW libGLFW3.a GLFW3 libglfw.so libglfw.so.3 libglfw.so.3.0
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		)
ENDIF(WIN32)

# message(STATUS "GLFW3 lib: ${GLFW3_LIBRARY}, include: ${GLFW3_INCLUDE_DIRS}")

IF(GLFW3_LIBRARY AND GLFW3_INCLUDE_DIRS)
	SET(GLFW3_FOUND TRUE)
	SET(GLFW_INCLUDE_DIRS ${GLFW3_INCLUDE_DIRS})
	SET(GLFW_LIBRARY_DIRS ${GLFW3_LIBRARY})
	message(STATUS "Found GLFW3: ${GLFW3_LIBRARY}")
ELSE()
	message(STATUS "GLFW3 NOT found!")
ENDIF(GLFW3_LIBRARY AND GLFW3_INCLUDE_DIRS)
