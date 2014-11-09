#include "xml_doc.hpp"
#include "xml_exception.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "bilanz.hpp"

typedef enum { NON_RECURSIVE = 0, RECURSIVE = 1 } XmlCopy;
typedef enum { KEEP_BLANKS = 0, INDENT = 1 } XmlFormat;

XmlDoc::XmlDoc( ) : xmlDoc_{nullptr}, pathname_{} {}

XmlDoc::XmlDoc( const Pathname &pathname ) : pathname_{pathname} {
    std::string fbuff{fileToString( pathname_.toString( ) )};
    handler_.registerHandler( );
    std::unique_ptr<xmlDoc, FreeXmlDoc> xml{xmlCtxtReadMemory(
        parserCtxt_.get( ), fbuff.data( ), fbuff.size( ), pathname_.toString( ).c_str( ),
        NULL, XML_PARSE_NONET | XML_PARSE_NOWARNING )};
    xmlDoc_ = std::move( xml );

    if ( !xmlDoc_ ) {
        std::ostringstream out( "XmlException: XML tree could not be created: [ ",
                                std::ios_base::ate );
        out << pathname_ << " ]\n";
        if ( handler_.hasErrors( ) ) {
            handler_.stream_to( out );
        }
        throw XmlException{out.str( )};
    }
}

XmlDoc::XmlDoc( xmlDocPtr xml ) : xmlDoc_{xml} {
    pathname_ = ( xml->name ) ? Pathname{std::string{xml->name}} : Pathname{};
}

XmlDoc::XmlDoc( const XmlDoc &doc )
    : xmlDoc_{xmlCopyDoc( doc.xmlDoc_.get( ), RECURSIVE )}, pathname_{doc.pathname_} {}

auto XmlDoc::operator=( const XmlDoc &xml ) -> XmlDoc & {
    XmlDoc tmp{xml};
    xmlDoc_ = std::move( tmp.xmlDoc_ );
    return *this;
}

auto XmlDoc::swap( XmlDoc &other ) -> void {
    std::swap( xmlDoc_, other.xmlDoc_ );
    std::swap( pathname_, other.pathname_ );
}

auto XmlDoc::toString( ) const -> std::string {
    if ( xmlDoc_ ) {
        xmlChar *buff;
        int size;
        xmlDocDumpFormatMemory( xmlDoc_.get( ), &buff, &size, INDENT );
        std::string strbuff{( char * )buff, static_cast<size_t>( size )};
        xmlFree( buff ); // free buff
        return strbuff;
    }
    return std::string{};
}

auto fileToString( const std::string &path ) -> std::string {
    std::ifstream in;
    in.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    in.open( path.data( ), std::ios::in | std::ios::binary );
    std::ostringstream contents;
    contents << in.rdbuf( );
    in.close( );
    return contents.str( );
}

auto operator<<( std::ostream &os, const XmlDoc &doc ) -> std::ostream & {
    os << doc.toString( );
    return os;
}

template <>
void std::swap( XmlDoc &lhs, XmlDoc &rhs ) {
    lhs.swap( rhs );
}
