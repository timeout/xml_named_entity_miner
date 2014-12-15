#pragma once

#include "xml_doc.hpp"

#include "libxml2_error_handlers.hpp"
#include <libxml/xmlschemas.h>

#include <iosfwd>
#include <memory>

class FreeXmlSchemaParser {
public:
    auto operator( )( xmlSchemaParserCtxt *schemaParserCtxt ) const -> void {
        xmlSchemaFreeParserCtxt( schemaParserCtxt );
    }
};

class XmlSchemaParser {
public:
    XmlSchemaParser( );
    explicit XmlSchemaParser( const XmlDoc &xml );
    XmlSchemaParser( const XmlSchemaParser & ) = delete;
    XmlSchemaParser &operator=( const XmlSchemaParser & ) = delete;
    XmlSchemaParser( XmlSchemaParser&&) = delete;
    XmlSchemaParser &operator=( XmlSchemaParser && ) = delete;
    auto get( ) const -> xmlSchemaParserCtxt * { return spc_.get( ); }
    friend auto operator>>( const XmlDoc &xml, XmlSchemaParser &xmlSchemaParser )
        -> XmlSchemaParser &;
    auto errorHandler( ) const -> const IErrorHandler & { return spch_; }

private:
    using SchemaParserCtxtT = std::unique_ptr<xmlSchemaParserCtxt, FreeXmlSchemaParser>;
    SchemaParserCtxtT spc_;
    XmlSchemaParserErrorHandler spch_;
};

