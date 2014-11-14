#include "xml_doc.hpp"
#include "xml_exception.hpp"
#include "xml_parser_ctxt.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>

#include <libxml/parser.h>

// #include "bilanz.hpp"

typedef enum { NON_RECURSIVE = 0, RECURSIVE = 1 } XmlCopy;
typedef enum { KEEP_BLANKS = 0, INDENT = 1 } XmlFormat;

static auto loadXml( std::istream &is, XmlErrorHandler& handler ) -> std::unique_ptr<xmlDoc, FreeXmlDoc>;

XmlDoc::XmlDoc( ) : xmlDoc_{nullptr} {}

XmlDoc::XmlDoc( std::istream &is ) : xmlDoc_{nullptr} {
    xmlDoc_ = loadXml(is, handler_);
}

// XmlDoc::XmlDoc( xmlDocPtr xml ) : xmlDoc_{xml} {
//     pathname_ = ( xml->name ) ? Pathname{std::string{xml->name}} : Pathname{};
// }

XmlDoc::XmlDoc( const XmlDoc &doc )
    : xmlDoc_{xmlCopyDoc( doc.xmlDoc_.get( ), RECURSIVE )} {}

XmlDoc::XmlDoc( XmlDoc &&doc )
    : xmlDoc_{std::move( doc.xmlDoc_ )}, handler_{std::move( doc.handler_ )} {}

auto XmlDoc::operator=( const XmlDoc &rhs ) -> XmlDoc & {
    XmlDoc tmp{rhs};
    xmlDoc_ = std::move( tmp.xmlDoc_ );
    return *this;
}

auto XmlDoc::operator=( XmlDoc &&rhs ) -> XmlDoc & {
    xmlDoc_ = std::move( rhs.xmlDoc_ );
    handler_ = std::move( rhs.handler_ );
    return *this;
}

auto XmlDoc::swap( XmlDoc &other ) -> void { std::swap( xmlDoc_, other.xmlDoc_ ); }

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

static auto loadXml( std::istream &is, XmlErrorHandler& handler )
    -> std::unique_ptr<xmlDoc, FreeXmlDoc> {
    std::istreambuf_iterator<char> eos;
    std::string fbuff{std::istreambuf_iterator<char>{is}, eos};
    handler.registerHandler( );
    XmlParserCtxt parser;
    return std::unique_ptr<xmlDoc, FreeXmlDoc>{
        xmlCtxtReadMemory( parser.get( ), fbuff.data( ), fbuff.size( ), NULL, NULL,
                           XML_PARSE_NONET | XML_PARSE_NOWARNING )};
}

auto operator>>( std::istream &is, XmlDoc &doc ) -> std::istream & {
    doc.xmlDoc_ = loadXml(is, doc.handler_);
    return is;
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

template <>
void std::swap( XmlDoc &lhs, XmlDoc &rhs ) {
    lhs.swap( rhs );
}
