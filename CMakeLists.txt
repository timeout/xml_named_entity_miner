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
    set(CMAKE_CXX_FLAGS "${warnings} ${cxxflag_options}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings} ${cflag_options}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(sample_RESOURCES "${CMAKE_SOURCE_DIR}/sample.qrc")

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

find_package(OpenGL REQUIRED) # this is only required with FreeBSD 10
find_package(Qt5Widgets REQUIRED)

qt5_wrap_cpp( GUI2_headers
    gui2/mainwindow.hpp
    gui2/xml_display.hpp
    gui2/xml_display_highlighter.hpp
    gui2/stacked_text_display.hpp
    gui2/text_display.hpp
    gui2/text_display_highlighter.hpp
    gui2/xml_file_explorer.hpp
    gui2/xml_file_outline.hpp
    gui2/line_edit.hpp
    gui2/xpath_query_widget.hpp
    gui2/tabbed_ontology_view.hpp
    gui2/ontology_view.hpp
    gui2/ontology_view_context_menu.hpp
    gui2/add_ontology_dialog.hpp
    gui2/xml_task_dialog.hpp
    gui2/xml_schema_dialog.hpp
    gui2/xslt_dialog.hpp
)

qt5_add_resources(RESOURCE_ADDED ${sample_RESOURCES})
include_directories(${QT_INCLUDES} ${OPENGL_INCLUDE_DIR})

# add_library(pathname_lib lib/pathname.cpp lib/pathname.hpp)
# add_library(dir_lib lib/dir.cpp lib/dir.hpp lib/singleton.hpp)
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
    lib/xpath_expression.hpp
    lib/xpath_query.hpp
    lib/xpath_query.cpp
    lib/xpath_result_set.hpp
    lib/xpath_result_set.cpp
    lib/xslt_doc.hpp
    lib/xml_element.hpp
    lib/xml_element.cpp
)

add_library( GUI2_library
    ${GUI2_headers}
    # ${RESOURCE_ADDED}
    gui2/mainwindow.cpp
    gui2/xml_display.cpp
    gui2/xml_display_highlighter.cpp
    gui2/stacked_text_display.cpp
    gui2/text_display.cpp
    gui2/text_display_highlighter.cpp
    gui2/xml_file_explorer.cpp
    gui2/xml_file_outline.cpp
    gui2/line_edit.cpp
    gui2/xpath_query_widget.cpp
    gui2/tabbed_ontology_view.cpp
    gui2/ontology_view.cpp
    gui2/ontology_view_context_menu.cpp
    gui2/add_ontology_dialog.cpp
    gui2/dictionary.hpp
    gui2/dictionary.cpp
    gui2/thesaurus.hpp
    gui2/thesaurus.cpp
    gui2/xml_task_dialog.cpp
    gui2/xml_schema_dialog.cpp
    gui2/xslt_dialog.cpp
)

qt5_use_modules(GUI2_library Widgets)
qt5_use_modules(xml_lib Core)

# target_link_libraries(dir_lib pathname_lib)
target_link_libraries(xml_lib 
    # pathname_lib
    utils_lib
    ${LIBXML2_LIBRARIES} 
    ${LIBXSLT_LIBRARIES}
    ${LIBXSLT_EXSLT_LIBRARIES} 
)
target_link_libraries(GUI2_library
    xml_lib
    utils_lib
)

include_directories(lib)
include_directories(gui2)

add_subdirectory(gtest-1.7.0)
add_subdirectory(tests)

add_executable(program.out main.cpp 
    #${RESOURCE_ADDED}
)
target_link_libraries(program.out 
    xml_lib
    GUI2_library
)

qt5_use_modules(program.out Widgets)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")

