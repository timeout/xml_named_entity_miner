#include "lib/bilanz.hpp"
#include "lib/xml_doc.hpp"
#include "lib/xml_schema_parser.hpp"
#include "lib/xml_schema.hpp"
#include <iostream>
#include <thread>
#include <fstream>

int main( ) {
    std::cout << "Hello World!" << std::endl;
    XmlDoc xml;
    std::cerr << "opening file..." << std::endl;
    std::ifstream f( "test.xml", std::ios::in );
    if ( f ) {
        f >> xml;
        std::cout << xml << std::endl;
    } else {
        std::cerr << "Could not open file." << std::endl;
    }

    std::ifstream g( "bilanz.xsd", std::ios::in );
    XmlDoc xml1{g};
    if ( xml1.errorHandler( ).hasErrors( ) ) {
        std::cerr << xml1.errorHandler( ) << std::endl;
        return 1;
    }
    std::cerr << xml1 << std::endl;

    XmlSchemaParserCtxt s;
    xml1 >> s;

    if ( s.errorHandler( ).hasErrors( ) ) {
        std::cerr << s.errorHandler( ) << std::endl;
    }

    XmlSchema schema;
    return 0;
}
