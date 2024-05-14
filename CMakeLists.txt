cmake_minimum_required(VERSION 3.28)

set(LIBRARY_NAME vector)
set(PROJECT_VERSION 1.10.1)
project(${LIBRARY_NAME} VERSION ${PROJECT_VERSION} LANGUAGES C DESCRIPTION "Vector container library.")

add_compile_definitions(LIBVECTOR_VERSION="${PROJECT_VERSION}")

add_library(${LIBRARY_NAME} SHARED
    include/logger.h
    include/vector.h
    src/logger.c
    src/vector.c)

include_directories(include)

set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${LIBRARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER include/vector.h)
configure_file(vector.pc.in vector.pc @ONLY)

include(GNUInstallDirs)

install(TARGETS ${LIBRARY_NAME} EXPORT ${LIBRARY_NAME}-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${LIBRARY_NAME}-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${LIBRARY_NAME}-${PROJECT_VERSION})

configure_file(
    ${${LIBRARY_NAME}_SOURCE_DIR}/${LIBRARY_NAME}-config.cmake.in
    ${${LIBRARY_NAME}_BINARY_DIR}/${LIBRARY_NAME}-config.cmake @ONLY)

configure_file(
    ${${LIBRARY_NAME}_SOURCE_DIR}/${LIBRARY_NAME}-config-version.cmake.in
    ${${LIBRARY_NAME}_BINARY_DIR}/${LIBRARY_NAME}-config-version.cmake @ONLY)

install(FILES ${${LIBRARY_NAME}_BINARY_DIR}/${LIBRARY_NAME}-config.cmake
    ${${LIBRARY_NAME}_BINARY_DIR}/${LIBRARY_NAME}-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${LIBRARY_NAME}-${PROJECT_VERSION})

install(FILES ${CMAKE_BINARY_DIR}/vector.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
