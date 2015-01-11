cmake_minimum_required(VERSION 3.0.2)

project(sample)
include(CheckIncludeFiles) # macro

set(CMAKE_CXX_FLAGS "-Wall -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(ADDITIONAL_LIBS "")

find_package(LibXml2 REQUIRED)
message(STATUS "LIBXML2_INCLUDE_DIR: " ${LIBXML2_INCLUDE_DIR})
include_directories(${LIBXML2_INCLUDE_DIR})
# list(APPEND ADDITIONAL_LIBS  ${LIBXML2_INCLUDE_DIR})
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

find_package(OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
qt5_wrap_cpp(QT_SRC gui/mainwindow.hpp gui/xml_tree.hpp)
include_directories(${QT_INCLUDES} ${OPENGL_INCLUDE_DIR})

add_library(pathname_lib lib/pathname.cpp lib/pathname.hpp)
add_library(dir_lib lib/dir.cpp lib/dir.hpp lib/singleton.hpp)
add_library(xml_lib 
    lib/xml_parser_ctxt.hpp 
    lib/libxml2_error_handlers.cpp
    lib/xml_string.hpp
    lib/xml_doc.hpp
    lib/xml_doc.cpp
    lib/xml_schema_parser.cpp
    lib/xml_schema_parser.hpp
    lib/xml_schema.hpp
    lib/xml_schema_validator.hpp
    lib/xpath_ctxt.hpp
    lib/xpath_ctxt.cpp
    lib/xpath_query.hpp
    lib/xpath_query.cpp
    lib/xpath_result_set.hpp
    lib/xpath_result_set.cpp
    lib/xslt_doc.hpp
    lib/xml_element.hpp
    lib/xml_element.cpp
)
add_library(gui_widgets 
    ${QT_SRC}
    gui/xml_tree.cpp
)
qt5_use_modules(gui_widgets Widgets)

target_link_libraries(dir_lib pathname_lib)
target_link_libraries(xml_lib 
    pathname_lib
    ${LIBXML2_LIBRARIES} 
    ${LIBXSLT_LIBRARIES}
    ${LIBXSLT_EXSLT_LIBRARIES} 
)
target_link_libraries(gui_widgets
    xml_lib
)

include_directories(lib)
include_directories(gui)

add_subdirectory(gtest-1.7.0)
add_subdirectory(tests)

add_executable(program.out main.cpp)
target_link_libraries(program.out 
    xml_lib
    gui_widgets
)
qt5_use_modules(program.out Widgets)

