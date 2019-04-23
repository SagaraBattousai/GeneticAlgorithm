cmake_minimum_required(VERSION 3.1...3.14)

configure_file(cmake/configureGoogletest.cmake googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)

if (result)
  message(FATAL_ERROR "Cmake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)

if (result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

#Building done: now setting up

#if(Win32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#endif()

#Don't understand Well enough
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

