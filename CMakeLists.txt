cmake_minimum_required(VERSION 3.1...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(GeneticAlgorithm
        VERSION 1.0.0
        DESCRIPTION "Genetic Algorithm Library."
        LANGUAGES CXX)

set(CMAKE_GENERATOR_PLATFORM, "Visual Studio 16 2019" x64)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)
add_subdirectory(app)


#################################

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
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

if(Win32)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)




