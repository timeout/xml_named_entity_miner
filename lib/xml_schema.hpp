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
    explicit XmlSchema( const XmlSchemaParser &spc ) {
        schema_ = loadSchema( spc, schemaHandler_ );
    }
    XmlSchema( const XmlSchema &) = delete;
    XmlSchema( XmlSchema &&) = delete;
    auto operator=( const XmlSchema & ) -> XmlSchema & = delete;
    auto operator=( XmlSchema && ) -> XmlSchema & = delete;
    auto get( ) const -> xmlSchema * { return schema_.get( ); }
    friend auto operator>>( XmlSchemaParser &spc, XmlSchema &schema ) -> XmlSchema & {
        schema.schema_ = loadSchema( spc, schema.schemaHandler_ );
        return schema;
    }
    auto errorHandler( ) const -> const IErrorHandler & { return schemaHandler_; }

private:
    using XmlSchemaT = std::unique_ptr<xmlSchema, FreeXmlSchema>;
    static auto loadSchema( const XmlSchemaParser &spc,
                            StandardErrorHandler &handler ) -> XmlSchemaT {
        XmlSchemaT tmp{xmlSchemaParse( spc.get( ) )};
        if ( !tmp ) {
            handler.message( "Could not create XmlSchema" );
        }
        return tmp;
    }

    XmlSchemaT schema_;
    StandardErrorHandler schemaHandler_;
};

