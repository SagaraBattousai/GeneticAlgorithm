
set(srcFiles geneticAlgorithm.cpp geneticAlgorithmFactory.cpp)
set(headerFiles gene.h geneticAlgorithm.h geneticAlgorithmFactory.h)

set(privateHeaderFiles ../include/Statistics/sampling.h)


list(TRANSFORM headerFiles PREPEND ../include/GeneticAlgorithm/)

add_library(GeneticAlgorithm SHARED ${srcFiles} ${headerFiles} ${privateHeaderFiles})
target_include_directories(GeneticAlgorithm PUBLIC ../include/GeneticAlgorithm)
target_include_directories(GeneticAlgorithm PRIVATE ../include/Statistics)

#target_link_libraries(GeneticAlgorithm PRIVATE Statistics)


if(MSVC)
  target_compile_options(GeneticAlgorithm PRIVATE /W4 /WX)
endif()

if(${CMAKE_VERSION} VERSION_GREATER 3.8)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "src" FILES ${srcFiles})
	#source_group(TREE ${CMAKE_SOURCE_DIR}/Statistics PREFIX "src" FILES ../Statistics/sampling.cpp)
	source_group(TREE ${CMAKE_SOURCE_DIR}/include/GeneticAlgorithm PREFIX "headers" FILES ${headerFiles})
	source_group(TREE ${CMAKE_SOURCE_DIR}/include/Statistics PREFIX "headers" FILES ${privateHeaderFiles})
	#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "headers" FILES ${privateHeaderFiles})

endif()


