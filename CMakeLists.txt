cmake_minimum_required(VERSION 3.3)
project(Communication)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# specify link directory
link_directories("/usr/local/lib/")
link_libraries(serial)

# add headers
file(GLOB_RECURSE HEADERS "include/*.h")
set (INCLUDE_DIRS "")
foreach (headerFile ${HEADERS})
    get_filename_component(dir ${headerFile} PATH)
    list (APPEND INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)
include_directories(${INCLUDE_DIRS})

# add source code
file(GLOB_RECURSE SOURCES "src/*.cpp")

# create examples
add_subdirectory(examples)

# create test
add_subdirectory(tests)




include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")