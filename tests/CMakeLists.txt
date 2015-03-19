find_package(Qt5Core REQUIRED)

include_directories(${gtest_SOURCE_DIR}/include)
set(test_sources gtest_main.cpp
    test_helper.hpp
    test_helper.cpp
    xml_element_test.cpp
    xml_doc_test.cpp
    xpath_query_test.cpp
    xpath_result_set_test.cpp
    xslt_doc_test.cpp
    # synonyms_test.cpp
    # pathname_test.cpp
    # dir_test.cpp
    xml_schema_parser_test.cpp
    xml_schema_test.cpp
    xml_schema_validator_test.cpp
    xpath_ctxt_test.cpp
    qregexp_test.cpp
    )
add_executable(run_tests.out ${test_sources})
qt5_use_modules(run_tests.out Core)
target_link_libraries(run_tests.out gtest_main 
    pathname_lib 
    dir_lib
    xml_lib 
)
