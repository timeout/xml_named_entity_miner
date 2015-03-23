
set( GUI2_headers
    mainwindow.hpp
    xml_display.hpp
    xml_display_highlighter.hpp
    stacked_text_display.hpp
    text_display.hpp
    text_display_highlighter.hpp
    xml_file_explorer.hpp
    xml_file_outline.hpp
    line_edit.hpp
    xpath_query_widget.hpp
    tabbed_ontology_view.hpp
    ontology_view.hpp
    ontology_view_context_menu.hpp
    add_ontology_dialog.hpp
    xml_task_dialog.hpp
    xml_schema_dialog.hpp
    xslt_dialog.hpp
)

add_library( GUI2_library
    ${GUI2_headers}
    mainwindow.cpp
    xml_display.cpp
    xml_display_highlighter.cpp
    stacked_text_display.cpp
    text_display.cpp
    text_display_highlighter.cpp
    xml_file_explorer.cpp
    xml_file_outline.cpp
    line_edit.cpp
    xpath_query_widget.cpp
    tabbed_ontology_view.cpp
    ontology_view.cpp
    ontology_view_context_menu.cpp
    add_ontology_dialog.cpp
    dictionary.hpp
    dictionary.cpp
    thesaurus.hpp
    thesaurus.cpp
    xml_task_dialog.cpp
    xml_schema_dialog.cpp
    xslt_dialog.cpp
)

target_link_libraries(GUI2_library
    xml_lib
    utils_lib
    Qt5::Widgets
    # ${RESOURCE_ADDED}
)

