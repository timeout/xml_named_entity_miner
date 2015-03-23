set(GTEST_DIR "../../gtest-1.7.0")

add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/gtest)
set_property(TARGET gtest APPEND_STRING PROPERTY COMPILE_FLAGS " -w")

include_directories(SYSTEM ${GTEST_DIR}/gtest/include)

find_package(Qt5Core REQUIRED)

include_directories(${gtest_SOURCE_DIR}/include)

set(test_sources 
    gtest_main.cpp
    test_helper.hpp
    test_helper.cpp
    xml_element_test.cpp
    xml_doc_test.cpp
    xpath_query_test.cpp
    xpath_result_set_test.cpp
    xslt_doc_test.cpp
    # xml_schema_parser_test.cpp
    # xml_schema_test.cpp
    # xml_schema_validator_test.cpp
    xpath_ctxt_test.cpp
    qregexp_test.cpp
)

add_executable(run_tests.out ${test_sources})

target_link_libraries(run_tests.out 
    gtest_main 
    xml_lib 
    Qt5::Core
)

add_test(run_tests.out run_tests.out)
