
# set executable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./")

# add executable
add_executable(main main.cpp)
target_link_libraries(main ${ARM_LIB})