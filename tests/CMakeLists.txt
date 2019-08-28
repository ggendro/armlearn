cmake_minimum_required(VERSION 3.3)
project(test)

# Single isolated cmake file used for testing purposes


# Import required libraries
link_directories("/usr/local/lib/")
link_libraries(serial)
link_libraries(orocos-kdl)
link_libraries(armlearn)

find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_DIRS})
link_libraries(${PYTHON_LIBRARIES})


# Create executable
add_executable(main main.cpp)
target_link_libraries(main)