

# enable google tests
include(GoogleTest)
enable_testing()

# set executable directory 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./")

# add coverage
SET(GCC_COVERAGE_COMPILE_FLAGS "--coverage")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# look for test files
file(GLOB_RECURSE TESTS_TO_BUILD "test_*.cpp")
foreach(example ${TESTS_TO_BUILD})
    get_filename_component(ex_name ${example} NAME_WE)
    get_filename_component(ex_path ${example} DIRECTORY)
    message(STATUS "Created test " ${ex_name} " from " ${ex_path} )
endforeach()

# create main test executable
add_executable(test_main ${TESTS_TO_BUILD} ${HEADERS} ${SOURCES})
target_link_libraries(test_main gtest)
target_link_libraries(test_main pthread)

# create tests
set(TestArgs --coverage)
gtest_add_tests(
    TARGET test_main
    EXTRA_ARGS "${TestArgs}"
    SOURCES "${TESTS_TO_BUILD}"
)

