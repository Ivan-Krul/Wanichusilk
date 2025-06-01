# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set a cache directory
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("Wanichusilk")

# Include sub-projects.
file(GLOB_RECURSE INCLUDES src/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
     add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
#elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${INCLUDES})
#endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

foreach(dir ${INCLUDES})
    get_filename_component(HEADER_DIR ${dir} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${HEADER_DIR})
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# Include modules
#find_package(JPEG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE JPEG::JPEG)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

find_package(SDL3_image CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)

#set(wxWidgets_ROOT_DIR "D:/Applicational Projects/PhotoManager/external/wxWidgets/")
#set(wxBUILD_SHARED OFF)
#add_subdirectory(external/wxWidgets)
#target_link_libraries(${PROJECT_NAME} PRIVATE wx::net wx::core wx::base)

#add_subdirectory(external/libwebp)
#target_link_libraries(${PROJECT_NAME} PRIVATE webp)

#add_subdirectory ("src" "./bin/")
