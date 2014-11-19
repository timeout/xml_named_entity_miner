#include "xslt_transform.hpp"

#include <fstream>

#include <libxml/tree.h>
#include <libexslt/exslt.h>
#include <libxslt/transform.h>

namespace bilanzXml {
XsltTransform::XsltTransform( const std::string &filename )
    : filename_( filename ) {
    std::string fbuff;
    try {
        fbuff = fileToString( filename );
    }
    catch ( std::ifstream::failure e ) {
        std::ostringstream out;
        out << "XsltException: Could not open file: [ " << filename << " ]"
            << std::endl;
        throw XsltException( out.str( ) );
    }
    catch ( ... ) {
        throw;
    }

    handler_.registerHandler( );
    xmlDocPtr xsltDoc = xmlCtxtReadMemory(
        parser_.context( ), fbuff.data( ), fbuff.size( ), filename.data( ),
        NULL, XML_PARSE_NONET | XML_PARSE_NOWARNING );
    if ( xsltDoc == NULL ) {
        std::ostringstream out(
            "XsltException: Xslt stylesheet parse error: [ ",
            std::ios_base::ate );
        out << filename << " ]";
        if ( handler_.hasErrors( ) ) {
            handler_.stream_to( out );
        }
        throw XsltException( out.str( ) );
    }

    exsltRegisterAll( );
    stylesheet_ = xsltParseStylesheetDoc( xsltDoc );
    if ( stylesheet_ == NULL ) {
        xmlFreeDoc( xsltDoc ); // free xsltDoc
        throw XsltException(
            "XsltException: Xslt stylesheet could not be parsed." );
    }
}

XmlDoc XsltTransform::transform( XmlDoc &xml ) const {
    xmlDocPtr trans = xsltApplyStylesheet( stylesheet_, xml.get(), NULL );
    if ( trans == NULL ) {
        std::ostringstream out;
        out << "XsltException: transform failed [ " << xml.filename() << " ]";
        throw XsltException( out.str( ) );
    }
    return XmlDoc( trans );
}
} // namespace bilanzXml
