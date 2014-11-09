#pragma once

#include <iosfwd>
#include <vector>
#include <string>

class IErrorHandler {
public:
    virtual ~IErrorHandler( ) = 0;
    virtual auto hasErrors( ) const -> bool = 0;
    virtual auto message( const std::string &msg ) -> void = 0;
    virtual auto stream_to( std::ostream &os ) const -> void = 0;
    virtual auto reset( ) -> void = 0;
};

inline IErrorHandler::~IErrorHandler( ) {}

inline std::ostream &operator<<( std::ostream &os, const IErrorHandler &handler ) {
    handler.stream_to( os );
    return os;
}
