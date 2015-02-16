cmake_minimum_required(VERSION 3.0.2)

project(sample)
include(CheckIncludeFiles) # macro

set(CMAKE_CXX_FLAGS "-Wall -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(sample_RESOURCES "${CMAKE_SOURCE_DIR}/sample.qrc")

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
qt5_wrap_cpp(QT_GUI_src 
    gui/mainwindow.hpp 
    gui/xml_file_outline.hpp
    gui/xml_display.hpp
    gui/xml_highlighter.hpp
    gui/txt_selection_display.hpp
    gui/element_selections.hpp
    gui/ontology.hpp
    gui/ontology_dialog.hpp
)
qt5_wrap_cpp(QT_SYN_LIB_src
    lib/synonyms.hpp
)
qt5_add_resources(RESOURCE_ADDED ${sample_RESOURCES})
include_directories(${QT_INCLUDES} ${OPENGL_INCLUDE_DIR})

add_library(pathname_lib lib/pathname.cpp lib/pathname.hpp)
add_library(dir_lib lib/dir.cpp lib/dir.hpp lib/singleton.hpp)
add_library(syn_lib ${QT_SYN_LIB_src} lib/synonyms.cpp)
add_library(utils_lib lib/utils.hpp lib/utils.cpp)
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
    ${QT_GUI_src}
    ${RESOURCE_ADDED}
    gui/xml_file_outline.cpp
    gui/xml_display.cpp
    gui/xml_highlighter.cpp
    gui/mainwindow.cpp
    gui/txt_selection_display.cpp
    gui/element_selections.cpp
    gui/ontology.cpp
    gui/ontology_dialog.cpp
)
qt5_use_modules(gui_widgets Widgets)
qt5_use_modules(xml_lib Core)
qt5_use_modules(syn_lib Core)

target_link_libraries(dir_lib pathname_lib)
target_link_libraries(xml_lib 
    pathname_lib
    ${LIBXML2_LIBRARIES} 
    ${LIBXSLT_LIBRARIES}
    ${LIBXSLT_EXSLT_LIBRARIES} 
)
target_link_libraries(gui_widgets
    xml_lib
    syn_lib
    utils_lib
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

