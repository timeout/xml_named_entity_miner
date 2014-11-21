#pragma once

#include "standard_error_handler.hpp"
#include <libxml/xmlerror.h>
#include <libxml/xmlschemas.h>
#include <libxml/xpath.h>

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

class XmlSchemaValidatorErrorHandler : public LibXml2ErrorHandler {
public:
    void registerHandler( xmlSchemaValidCtxt *schemaValidator ) {
        xmlSchemaSetValidStructuredErrors( schemaValidator, &LibXml2ErrorHandler::errors,
                                           static_cast<void *>( this ) );
    }
};

class XPathErrorHandler : public LibXml2ErrorHandler {
public:
    void registerHandler( xmlXPathContext *xpathctxt ) {
        xpathctxt->userData = this;
        xpathctxt->error = &LibXml2ErrorHandler::errors;
    }
};
