#pragma once

#include "standard_error_handler.hpp"
#include <libxml/xmlerror.h>
#include <libxml/xmlschemas.h>

class LibXml2ErrorHandler : public StandardErrorHandler {
public:
    static void errors( void *ctx, xmlErrorPtr p );
};

class XmlErrorHandler : public LibXml2ErrorHandler {
public:
    void registerHandler( ) {
        xmlSetStructuredErrorFunc( static_cast<void *>( this ),
                                   &LibXml2ErrorHandler::errors );
    }
};

class XmlSchemaParserErrorHandler : public LibXml2ErrorHandler {
public:
    void registerHandler( xmlSchemaParserCtxt *spcp ) {
        xmlSchemaSetParserStructuredErrors( spcp, &LibXml2ErrorHandler::errors,
                                            static_cast<void *>( this ) );
    }
};

