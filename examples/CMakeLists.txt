

# look for examples to build a CMakeLists.txt file for
file(GLOB_RECURSE EXAMPLES_TO_BUILD "example_*.cpp")

# build for each example
foreach(example ${EXAMPLES_TO_BUILD})
    get_filename_component(ex_name ${example} NAME_WE)
    get_filename_component(ex_path ${example} DIRECTORY)

    # write CMakLists.txt file
    file(WRITE ${ex_path}/CMakeLists.txt 
"
# auto-generated file

# set executable directory \n
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY \"./\")

# add executable
add_executable(" ${ex_name} " " ${ex_name} ".cpp)
target_link_libraries(" ${ex_name} " \${ARM_LIB})
"   )

    message(STATUS "Added CMakeLists.txt in " ${ex_path} " for generation of executable " ${ex_name})

    # generate executable
    add_subdirectory("${ex_path}")
endforeach()

