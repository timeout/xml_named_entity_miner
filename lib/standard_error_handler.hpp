#pragma once

#include "interface_error_handler.hpp"
#include <vector>
#include <string>
#include <ostream>
#include <iterator>

class StandardErrorHandler : public IErrorHandler {
public:
    StandardErrorHandler( ) = default;
    StandardErrorHandler( const StandardErrorHandler & ) = default;
    StandardErrorHandler( StandardErrorHandler && ) = default;
    virtual ~StandardErrorHandler( ) {}
    auto operator=( const StandardErrorHandler & ) -> StandardErrorHandler & = default;
    auto operator=( StandardErrorHandler && ) -> StandardErrorHandler & = default;
    auto hasErrors( ) const -> bool override { return !errors_.empty( ); }
    auto message( const std::string &msg ) -> void override { errors_.push_back( msg ); }
    auto stream_to( std::ostream &os ) const -> void override {
        std::copy( errors_.begin( ), errors_.end( ),
                   std::ostream_iterator<std::string>( os ) );
    }
    auto reset( ) -> void override { errors_.clear( ); }

private:
    std::vector<std::string> errors_;
};

