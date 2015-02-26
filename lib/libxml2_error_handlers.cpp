#include "libxml2_error_handlers.hpp"

#include <iostream>
#include <sstream>

auto LibXml2ErrorHandler::errors( void *ctx, xmlErrorPtr p ) -> void {
    if ( p->level != XML_ERR_NONE ) {
        std::ostringstream out;
        if ( p->file ) {
            std::string file{p->file};
            out << "Error in file [ " << file << " ]: ";
        }

        if ( !p->message ) {
            std::cerr << "DEBUG: no error message :(" << std::endl;
            return;
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

