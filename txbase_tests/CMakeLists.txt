cmake_minimum_required (VERSION 3.2)

set(BUILD_TARGET txbase_tests)

# googletest
add_subdirectory(libs/gtest)
find_package(Threads)

file(GLOB_RECURSE BUILD_SOURCE "./*.cc")
message(STATUS "txbase test sources: " ${BUILD_SOURCE})
add_executable(${BUILD_TARGET} ${BUILD_SOURCE})

add_dependencies(${BUILD_TARGET} googletest)

target_include_directories(${BUILD_TARGET} PUBLIC ${GTEST_INCLUDE_DIRS} ..)

if(NOT WIN32 OR MINGW)
	set(CAT_CMD cat)
	target_link_libraries(${BUILD_TARGET}
		${GTEST_LIBS_DIR}/libgtest.a
		${GTEST_LIBS_DIR}/libgtest_main.a
		)
else()
	set(CAT_CMD type)
	target_link_libraries(${BUILD_TARGET}
		debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
		optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
		)
	target_link_libraries(${BUILD_TARGET}
		debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
		optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
		)
endif()
target_link_libraries(${BUILD_TARGET} txbase)
target_link_libraries(${BUILD_TARGET} ${CMAKE_THREAD_LIBS_INIT})

add_test(
	NAME UnitTesting
	COMMAND ${BUILD_TARGET}
	)

# Run tests as part of build
add_custom_command(
	TARGET ${BUILD_TARGET}
	COMMENT "Run ${BUILD_TARGET}"
	POST_BUILD
	COMMAND ${BUILD_TARGET} #--gtest_filter=MathUtilTests.* > ${OUTPUT_FILE} || (cat ${OUTPUT_FILE} && false)
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	USES_TERMINAL
)
