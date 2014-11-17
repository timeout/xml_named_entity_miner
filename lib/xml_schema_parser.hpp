#pragma once

#include "xml_doc.hpp"

#include "libxml2_error_handlers.hpp"
#include <libxml/xmlschemas.h>

#include <iosfwd>
#include <memory>

class FreeXmlSchemaParserCtxt {
public:
    auto operator( )( xmlSchemaParserCtxt *schemaParserCtxt ) const -> void {
        xmlSchemaFreeParserCtxt( schemaParserCtxt );
    }
};

class XmlSchemaParserCtxt {
public:
    XmlSchemaParserCtxt( );
    explicit XmlSchemaParserCtxt( const XmlDoc &xml );
    XmlSchemaParserCtxt( const XmlSchemaParserCtxt & ) = delete;
    XmlSchemaParserCtxt &operator=( const XmlSchemaParserCtxt & ) = delete;
    auto get( ) const -> xmlSchemaParserCtxt * { return spc_.get( ); }
    friend auto operator>>( const XmlDoc &xml, XmlSchemaParserCtxt &xmlSchemaParser )
        -> XmlSchemaParserCtxt &;
    auto errorHandler( ) const -> const IErrorHandler & { return spch_; }

private:
    using SchemaParserCtxtT = std::unique_ptr<xmlSchemaParserCtxt, FreeXmlSchemaParserCtxt>;
    SchemaParserCtxtT spc_;
    XmlSchemaParserErrorHandler spch_;
};

