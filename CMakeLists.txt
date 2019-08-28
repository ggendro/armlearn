cmake_minimum_required(VERSION 3.3)
project(armlearn VERSION 1.0.1 DESCRIPTION "Library for communication and serial control with a robotic arm using Reinforcement Learning.")

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
link_libraries(orocos-kdl)

# include openmp
find_package(OpenMP)
if (OPENMP_FOUND)
    link_libraries(${OpenMP_CXX_LIBRARIES})
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# include python
find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_DIRS})
link_libraries(${PYTHON_LIBRARIES})


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


# create library
set(ARM_LIB armlearn)
include(GNUInstallDirs)
add_library(${ARM_LIB} SHARED
    ${HEADERS}
    ${SOURCES}
)
set_target_properties(armlearn PROPERTIES PUBLIC_HEADER "${HEADERS}")
target_include_directories(armlearn PRIVATE include)
target_include_directories(armlearn PRIVATE src)
install(TARGETS ${ARM_LIB} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${ARM_LIB})

# create examples
add_subdirectory(examples)

# create test
add_subdirectory(tests)

# create preesm target
add_subdirectory(preesm/org.ietr.preesm.reinforcement_learning/Spider)


include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

