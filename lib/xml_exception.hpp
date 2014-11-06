#pragma once

#include <stdexcept>
#include <string>

class XmlException : public std::runtime_error {
public:
    XmlException( ) : std::runtime_error( "XmlException" ) {}
    XmlException( const std::string &error ) : std::runtime_error( error ) {}
};

