# 'Google_test' is the subproject name
project(Rpn_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
add_executable(Rpn_Tests_run op_test.cpp token_test.cpp rpn_test.cpp)
target_include_directories(Rpn_Tests_run PUBLIC
        ../include
        ../../../include)
target_link_libraries(Rpn_Tests_run gtest gtest_main)