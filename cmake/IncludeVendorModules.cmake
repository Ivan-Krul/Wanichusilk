cmake_minimum_required (VERSION 3.10)

add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/SDL3")# EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/SDL3_image")# EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)

set(SDLTTF_VENDORED ON)
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/SDL3_ttf")# EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_ttf::SDL3_ttf)

#if(WIN32)
#	add_custom_target(CopyBinaries
#		COMMAND ${CMAKE_COMMAND} -E copy_directory ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin ${CMAKE_CURRENT_BINARY_DIR}
#	)
#	add_dependencies(${PROJECT_NAME} CopyBinaries)
#endif()

