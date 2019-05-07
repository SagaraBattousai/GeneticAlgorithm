cmake_minimum_required(VERSION 3.1...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(GeneticAlgorithm
        VERSION 1.0.0
        DESCRIPTION "Genetic Algorithm Library."
        LANGUAGES CXX)

if (MSVC)
	string(REGEX REPLACE "(.*)/W3(.*)" "\\1\\2" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#add_subdirectory(Statistics)
add_subdirectory(src)
add_subdirectory(app)


include(cmake/buildGoogleTest.cmake)
add_subdirectory(tests)


