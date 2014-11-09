#include "standard_error_handler.hpp"

#include <libxml/xmlerror.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

void StandardErrorHandler::message( const std::string &msg ) {  }

void StandardErrorHandler::stream_to( std::ostream &os ) const {
}

void StandardErrorHandler::reset( ) { }

