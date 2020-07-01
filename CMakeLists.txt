cmake_minimum_required (VERSION 3.12)
project (TestApp)
enable_language(C CXX)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package (Threads REQUIRED)
find_package (Protobuf REQUIRED)
            

option(WITH_TEST 		"build test" 				ON)
option(WITH_COVERAGE 	"generate coverage report" 	ON)
option(WITH_DOC 		"generate doxygen report" 	ON)


if(WITH_TEST)
	enable_testing()
endif()

if(WITH_COVERAGE)
	include(CodeCoverage)
endif()



add_subdirectory(Common)

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(utest)


if(WITH_DOC)
	find_package (Doxygen)
	if(${CMAKE_COMPILER_ID} MATCHES GNU AND $my_project_WITH_PROF)
		set_target_properties(my_prog
			PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -gp -fprofile-coverage -fprofile-args" LINK_FLAGS "-fprofile-arcs")
	endif()

	if(DOXYGEN_FOUND)
		if(DOXYGEN_DOT_FOUND)
			set(DOXYGEN_CONFIGURE_HAVE_DOT "HAVE_DOT=YES")
			get_filename_component(DOXYGEN_DOT_PATH "${DOXYGEN_DOT_EXECUTABLE}" DIRECTORY)
			file(TO_NATIVE_PATH "${DOXYGEN_DOT_PATH}" DOXYGEN_DOT_PATH)
			set(DOXYGEN_CONFIGURE_DOT_PATH "DOT_PATH=${DOXYGEN_DOT_PATH}")
		endif()
		add_custom_target(doc
			configure_file(Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
			${DOXYGEN_EXECUTABLE}
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMENT "Running Doxygen" VERBATIM
		)
	endif()
endif()


