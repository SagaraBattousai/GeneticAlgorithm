
set(srcFiles main.cpp)

add_executable(main ${srcFiles})
#target_link_libraries(main PUBLIC Statistics)

target_include_directories(main PUBLIC ../include/GeneticAlgorithm ../include/Statistics)

target_compile_definitions(main PRIVATE GENETIC_DIAGNOSTICS)

if(MSVC)
  target_compile_options(main PRIVATE /W4 /WX)
endif()


#add_custom_command(TARGET main POST_BUILD        # Adds a post-build event to MyTest
#	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
#	"${PROJECT_BINARY_DIR}/Statistics/Statistics.dll"      # <--this is in-file
#	"${PROJECT_BINARY_DIR}/app")   
