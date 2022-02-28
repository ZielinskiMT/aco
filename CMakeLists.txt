cmake_minimum_required(VERSION 3.18.4)

project(aco VERSION 0.1.0 LANGUAGES CXX)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# headers
set( Headers

)

# source
set( Sources

)

# tests
set( TestSources
    tests_aco.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${Headers} ${Sources})
add_executable(tests ${TestSources} ${Headers} ${Sources})

add_subdirectory(googletest)

enable_testing()

target_link_libraries(
    tests
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
