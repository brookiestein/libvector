cmake_minimum_required(VERSION 3.28)

set(LIBRARY_NAME vector)
set(PROJECT_VERSION 1.0.1)
project(${LIBRARY_NAME} VERSION ${PROJECT_VERSION} LANGUAGES C DESCRIPTION "Vector container library.")

add_library(${LIBRARY_NAME} SHARED
    src/logger.c
    src/vector.c)

set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${LIBRARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER src/vector.h)
configure_file(vector.pc.in vector.pc @ONLY)

include(GNUInstallDirs)

install(TARGETS ${LIBRARY_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/vector.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
