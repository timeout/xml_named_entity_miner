#pragma once

#include "xml_schema_parser.hpp"
#include <memory>
#include <libxml/xmlschemas.h>

class FreeXmlSchema {
public:
    auto operator( )( xmlSchema *schema ) const -> void { xmlSchemaFree( schema ); }
};

class XmlSchema {
public:
    XmlSchema( ) : schema_{nullptr} {}
    explicit XmlSchema( const XmlSchemaParserCtxt &spc )
        : schema_{xmlSchemaParse( spc.get( ) )} {
        if ( !schema_ ) {
            schemaHandler_.message( "this and that" );
        }
    }

    XmlSchema( const XmlSchema &other ) = delete;
    auto operator=( const XmlSchema & ) -> XmlSchema & = delete;

    auto get( ) const -> xmlSchema * { return schema_.get( ); }
    friend auto operator>>( XmlSchemaParserCtxt &spc, XmlSchema &schema ) -> XmlSchema & {
        XmlSchemaT tmp{xmlSchemaParse( spc.get( ) )};
        schema.schema_ = std::move( tmp );
        if ( !schema.schema_ ) {
            schema.schemaHandler_.message( "this and that" );
        }
        return schema;
    }
    auto errorHandler( ) const -> const IErrorHandler & { return schemaHandler_; }

private:
    using XmlSchemaT = std::unique_ptr<xmlSchema, FreeXmlSchema>;
    XmlSchemaT schema_;
    StandardErrorHandler schemaHandler_;
};

