
set(srcFiles main.cpp)

add_executable(main ${srcFiles})
# target_link_libraries(main PUBLIC CardGames)
# target_link_libraries(main PUBLIC Baccarat)

target_include_directories(main PUBLIC ../include/GeneticAlgorithm)

# if(${CMAKE_VERSION} VERSION_GREATER 3.8)
# 	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "src" FILES ${srcFiles})
# endif()

# add_custom_command(TARGET main POST_BUILD        # Adds a post-build event to MyTest
# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
# 		"${PROJECT_BINARY_DIR}/CardGames/CardGames.dll"      # <--this is in-file
# 		"${PROJECT_BINARY_DIR}/app")   
