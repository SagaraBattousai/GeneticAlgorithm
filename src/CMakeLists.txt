
set(srcFiles geneticAlgorithm.cpp geneticAlgorithmFactory.cpp)
set(headerFiles gene.h geneticAlgorithm.h geneticAlgorithmFactory.h)

list(TRANSFORM headerFiles PREPEND ../include/GeneticAlgorithm/)

add_library(GeneticAlgorithm SHARED ${srcFiles} ${headerFiles})
target_include_directories(GeneticAlgorithm PUBLIC ../include/GeneticAlgorithm)


if(${CMAKE_VERSION} VERSION_GREATER 3.8)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "src" FILES ${srcFiles})
	source_group(TREE ${CMAKE_SOURCE_DIR}/include/GeneticAlgorithm PREFIX "headers" FILES ${headerFiles})  
endif()


