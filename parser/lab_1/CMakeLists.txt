#cmake_minimum_required(VERSION 3.5)
#project(Lab_1)
#set(CMAKE_CXX_STANDARD 17)
#add_subdirectory(googletest)
#include_directories(googletest/googletest/include)
#add_executable(Lab_1
#        main.cpp
#        Trit.cpp
#        Trit.h
#        TritSet.cpp
#        TritSet.h tests/TritSetFixtures.cpp)
#target_link_libraries(Lab_1 gtest gtest_main)


cmake_minimum_required(VERSION 3.5)
project(Lab_1)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 14)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(Lab_1
        main.cpp
        Trit.cpp
        Trit.h
        TritSet.cpp
        TritSet.h
        tests/TritSetFixtures.cpp)

target_link_libraries(Lab_1 ${GTEST_LIBRARIES} pthread)