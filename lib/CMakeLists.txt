find_package(Qt5Core REQUIRED)

add_library(utils_lib 
    utils.hpp 
    utils.cpp
)

add_library(xml_lib 
    xml_parser_ctxt.hpp 
    libxml2_error_handlers.cpp
    xml_string.hpp
    xml_doc.hpp
    xml_doc.cpp
    xml_schema_parser.cpp
    xml_schema_parser.hpp
    xml_schema.hpp
    xml_schema_validator.hpp
    xpath_ctxt.hpp
    xpath_ctxt.cpp
    xpath_expression.hpp
    xpath_query.hpp
    xpath_query.cpp
    xpath_result_set.hpp
    xpath_result_set.cpp
    xslt_doc.hpp
    xml_element.hpp
    xml_element.cpp
    ${RESOURCE_ADDED}
)

target_link_libraries(xml_lib 
    utils_lib
    ${LIBXML2_LIBRARIES} 
    ${LIBXSLT_LIBRARIES}
    ${LIBXSLT_EXSLT_LIBRARIES} 
    Qt5::Core
)

add_subdirectory(tests)

