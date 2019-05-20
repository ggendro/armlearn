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

# add the executable
add_executable(main src/main.cpp)

# add the binary tree to the search path for include files
target_include_directories(main PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

target_link_libraries(main serial)

# add the install targets
install(TARGETS main DESTINATION bin)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
#link_directories("/usr/local")