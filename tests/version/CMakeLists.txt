cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME GetVersion)
project(${PROJECT_NAME} LANGUAGES C)

set(PROJECT_SOURCES main.c)

find_package(Vector REQUIRED)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} vector)
