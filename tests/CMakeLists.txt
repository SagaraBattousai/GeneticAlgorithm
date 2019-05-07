set(testFiles gene_tests.cpp)



add_executable(Tests ${testFiles})

target_include_directories(Tests PRIVATE ../include/GeneticAlgorithm)

target_link_libraries(Tests gtest_main)

add_test(NAME GeneMutatesOnlyOneBit COMMAND Tests)

if(MSVC)
  target_compile_options(Tests PRIVATE /W4 /WX)
endif()


if(${CMAKE_VERSION} VERSION_GREATER 3.8)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "tests" FILES ${testFiles})
endif()