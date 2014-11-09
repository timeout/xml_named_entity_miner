#pragma once

#include "standard_error_handler.hpp"
#include <libxml/xmlerror.h>

class LibXml2ErrorHandler : public StandardErrorHandler {
public:
    static void errors( void *ctx, xmlErrorPtr p );
};

class XmlErrorHandler : public LibXml2ErrorHandler {
public:
    void registerHandler( );
};
