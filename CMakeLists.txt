cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Xml Named Entity Miner")

include(CheckIncludeFiles) # macro

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
    set(cxxflag_options "-pedantic -std=c++11")
    set(cflag_options "-pedantic -std=c11")
    message(STATUS "Warnings: ${warnings} ${cxxflag_options}")
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings} ${cxxflag_options}")
    set(CMAKE_C_FLAGS   "${warnings} ${cflag_options}")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(LibXml2 REQUIRED)
message(STATUS "LIBXML2_INCLUDE_DIR: " ${LIBXML2_INCLUDE_DIR})
include_directories(${LIBXML2_INCLUDE_DIR})
find_path(ICONV_INCLUDE_DIR iconv.h)
include_directories(${ICONV_INCLUDE_DIR})
set(CMAKE_REQUIRED_INCLUDES ${ICONV_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})
check_include_files("libxml/tree.h" HAVE_LIBXML_TREE_H)
set(CMAKE_REQUIRED_INCLUDES "")

find_package(LibXslt REQUIRED)
message(STATUS "LIBXSLT_INCLUDE_DIR: " ${LIBXSLT_INCLUDE_DIR})
message(STATUS "LIBXSLT_LIBRARIES: " ${LIBXSLT_LIBRARIES})
message(STATUS "LIBXSLT_EXSLT_LIBRARIES: " ${LIBXSLT_EXSLT_LIBRARIES})
include_directories(${LIBXSLT_INCLUDE_DIR})
check_include_files("transform.h" HAVE_LIBXSLT_TRANSFORM_H)
set(CMAKE_REQUIRED_INCLUDES "")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

enable_testing()
include(gtest)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(lib)
include_directories(lib)

find_package(OpenGL REQUIRED) # this is only required with FreeBSD 10
find_package(Qt5Widgets REQUIRED)
include_directories(${QT_INCLUDES} ${OPENGL_INCLUDE_DIR})

set(qt_RESOURCES "${CMAKE_SOURCE_DIR}/gui2/icons/xml_named_entity_miner.qrc")
qt5_add_resources(RESOURCE_ADDED ${qt_RESOURCES})

add_subdirectory(gui2)
include_directories(gui2)

add_executable(program.out main.cpp) 
target_link_libraries(program.out 
    xml_lib
    GUI2_library
    Qt5::Widgets
)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "configure cflags first time cmake is run" FORCE)

