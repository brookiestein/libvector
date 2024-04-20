cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME StringVectorTest)
project(${PROJECT_NAME} LANGUAGES C)

set(PROJECT_SOURCES string_vector_test.c)

find_package(Vector REQUIRED)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} vector)
