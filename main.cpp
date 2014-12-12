#include "lib/bilanz.hpp"
#include "lib/xml_doc.hpp"
#include "lib/xml_schema_parser.hpp"
#include "lib/xml_schema.hpp"
#include "lib/xml_schema_validator.hpp"
#include "lib/xpath_ctxt.hpp"
#include "lib/xpath_query.hpp"
#include "lib/xpath_result_set.hpp"
#include "lib/xml_node.hpp"
// #include "lib/xslt_doc.hpp"
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

    XmlSchemaParser s;
    xml1 >> s;

    if ( s.errorHandler( ).hasErrors( ) ) {
        std::cerr << s.errorHandler( ) << std::endl;
    }

    if ( xml1 ) {
        std::cerr << "xml1 is not empty" << std::endl;
    }

    XmlSchema schema;
    s >> schema;

    XmlSchemaValidator validator;
    schema >> validator;
    if ( validator.errorHandler( ).hasErrors( ) ) {
        std::cerr << validator.errorHandler( ) << std::endl;
    }
    if ( !validator.validate( xml ) ) { // xml doesn't exist
        std::cerr << "Could not validate xml" << std::endl;
    }

    xml >> s >> schema >> validator;
    if ( xml.errorHandler( ).hasErrors( ) ) {
        std::cerr << "xml has errors" << std::endl;
    }
    if ( !xml ) {
        std::cerr << "empty doc" << std::endl;
    }

    // XsltDoc st;
    // XmlDoc bilanzStylesheet;
    std::ifstream sf{"transformations/bilanz_nest.xsl", std::ios::in};
    XmlDoc data;
    std::ifstream df{"transformations/Jean Studer invalid.xml", std::ios::in};
    df >> data;
    // sf >> bilanzStylesheet;
    // XmlDoc tr;
    // if ( bilanzStylesheet && data ) {
    //     bilanzStylesheet >> st;
    //     tr = st.transform( data );
    // }
    // std::cout << tr << std::endl;

    XPathCtxt ctxt{data};
    // data >> ctxt;
    // if ( tr ) {
    //     std::cerr << "tr tests true" << std::endl;
    // }
    if ( ctxt ) {
        std::cerr << "ctxt tests true" << std::endl;
    }
    // XPathQuery query;
    // ctxt >> query;
    // query.query( "//something" );
    if ( ctxt.errorHandler( ).hasErrors( ) ) {
        std::cerr << "ctxt has errors" << std::endl;
        std::cerr << ctxt.errorHandler( ) << std::endl;
    }
    //     XPathCtxt cp{ctxt};
    //     cp >> query;
    //     query.query( "//else" );
    //
    //     XPathCtxt ass;
    //     ass = ctxt;
    //     ass >> query;

    // query.query( "//paragraph" );

    // XPathResultSet rs( query );
    // std::cerr << "rs size: " << rs.size( ) << std::endl;

    XPathQuery q = ctxt.makeQuery( );
    q.query( "//paragraph" );
    XPathResultSet rs{q};
    std::cerr << "result set size: " << rs.size( ) << std::endl;
    // XmlNode node{rs[0]};
    for (int i = 0; i < rs.size(); ++i) {
        std::cerr << rs[i] << std::endl;
    }

    return 0;
}
