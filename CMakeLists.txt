cmake_minimum_required(VERSION 3.0.2)

project(sample)
include(CheckIncludeFiles) # macro

set(CMAKE_CXX_FLAGS "-Wall -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# set(ADDITIONAL_LIBS "")

find_package(LibXml2 REQUIRED)
message(STATUS "LBXML2_INCLUDE_DIR: " ${LIBXML2_INCLUDE_DIR})
include_directories(${LIBXML2_INCLUDE_DIR})
# list(APPEND ADDITIONAL_LIBS  ${LIBXML2_INCLUDE_DIR})
find_path(ICONV_INCLUDE_DIR iconv.h)
include_directories(${ICONV_INCLUDE_DIR})
set(CMAKE_REQUIRED_INCLUDES ${ICONV_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})
check_include_files("libxml/tree.h" HAVE_LIBXML_TREE_H)
set(CMAKE_REQUIRED_INCLUDES "")

add_library(pathname_lib lib/pathname.cpp lib/pathname.hpp)
add_library(dir_lib lib/dir.cpp lib/dir.hpp lib/singleton.hpp)
add_library(xml_lib 
    lib/xml_parser_ctxt.hpp 
    lib/libxml2_error_handlers.cpp
    lib/xml_doc.hpp
    lib/xml_doc.cpp
    lib/xml_schema_parser.cpp
    lib/xml_schema_parser.hpp
    lib/xml_schema.hpp
    lib/xml_schema_validator.hpp
)

target_link_libraries(dir_lib pathname_lib)
target_link_libraries(xml_lib ${LIBXML2_LIBRARIES} pathname_lib)

include_directories(lib)

add_subdirectory(gtest-1.7.0)
add_subdirectory(tests)

add_executable(program.out main.cpp)
target_link_libraries(program.out 
    xml_lib
)

