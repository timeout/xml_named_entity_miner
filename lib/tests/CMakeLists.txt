
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

# add_executable(run_tests.out ${test_sources})

add_gtest(run_tests.out ${test_sources})
target_link_libraries(run_tests.out 
    xml_lib 
    Qt5::Core
)

