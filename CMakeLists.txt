cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

project(sample)

find_package(LibXml2 REQUIRED)
message(STATUS "LBXML2_INCLUDE_DIR: " ${LIBXML2_INCLUDE_DIR})

add_library(pathname_lib lib/pathname.cpp lib/pathname.hpp)
add_library(dir_lib lib/dir.cpp lib/dir.hpp lib/singleton.hpp)
add_library(logger_lib 
    lib/logger.hpp 
    lib/logger.cpp 
    lib/log.hpp
    lib/log.cpp
)
add_library(xml_lib 
    lib/xml_parser_ctxt.hpp 
    lib/libxml2_error_handlers.cpp
    lib/xml_doc.hpp
    lib/xml_doc.cpp
)

target_link_libraries(dir_lib pathname_lib)
target_link_libraries(xml_lib ${LIBXML2_LIBRARIES} pathname_lib)

include_directories(lib)
include_directories(${LIBXML2_INCLUDE_DIR})

add_subdirectory(gtest-1.7.0)
add_subdirectory(tests)

add_executable(program.out main.cpp)
target_link_libraries(program.out logger_lib xml_lib pthread)

