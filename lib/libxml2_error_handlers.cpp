#include "libxml2_error_handlers.hpp"

#include <iostream>
#include <sstream>

void LibXml2ErrorHandler::errors( void *ctx, xmlErrorPtr p ) {
    if ( p->level != XML_ERR_NONE ) {
        std::ostringstream out;
        if ( p->file ) {
            std::string file{p->file};
            out << "Error in file [ " << file << " ]: ";
        }
        std::string msg{p->message};
        out << msg;
        IErrorHandler *handler = static_cast<IErrorHandler *>( ctx );
        if ( handler ) {
            handler->message( out.str( ) );
        } else {
            std::cerr << out.str( );
        }
    }
}

void XmlErrorHandler::registerHandler( ) {
    xmlSetStructuredErrorFunc( static_cast<void *>( this ),
                               &LibXml2ErrorHandler::errors );
}

void SchemaParserErrorHandler::registerHandler( xmlSchemaParserCtxtPtr &schemaParser ) {
    xmlSchemaSetParserStructuredErrors( schemaParser, &LibXml2ErrorHandler::errors,
                                        static_cast<void *>( this ) );
}

