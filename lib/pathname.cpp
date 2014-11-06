#include "pathname.hpp"
#include "bilanz.hpp"

#include <iostream>
#include <stdexcept>

enum SplitType { Basename, Dirname };

static std::string dirBaseSplit( const std::string path, const SplitType splitType );

Pathname::Pathname( ) : path_{"?"} {}

Pathname::Pathname( const std::string &path ) : path_{path} {
    if ( path_.empty( ) ) {
        throw std::runtime_error( "Invalid path" );
    }
    std::string::const_reverse_iterator last = path_.rbegin( );
    if ( *last == '/' && ++last != path_.rend( ) ) {
        path_.pop_back( );
    }
}

auto Pathname::isValid( ) const -> bool { return ( path_ != "?" ); }

auto Pathname::basename( ) const -> Pathname {
    if ( path_ == "?" ) {
        throw std::runtime_error( "Invalid path" );
    }
    return dirBaseSplit( path_, Basename );
}

auto Pathname::dirname( ) const -> Pathname {
    if ( path_ == "?" ) {
        throw std::runtime_error( "Invalid path" );
    }
    return dirBaseSplit( path_, Dirname );
}

auto Pathname::operator+=( const Pathname &rhs ) -> Pathname & {
    if ( path_ == "?" ) {
        throw std::runtime_error( "Invalid path" );
    }
    if ( path_ != "/" ) {
        path_.push_back( '/' );
    }
    if ( rhs.isRelative( ) ) {
        path_ += rhs.path_;
    } else {
        path_ = rhs.path_;
    }
    return *this;
}

// auto Pathname::operator+( const Pathname &component ) -> Pathname & {
//     operator+( component.toString( ) );
//     return *this;
// }
//
// auto Pathname::operator+( const std::string &component ) -> Pathname & {
//     if ( '/' == *component.begin( ) ) {
//         path_ = component;
//     } else {
//         path_.push_back( '/' );
//         path_ += component;
//     }
//     return *this;
// }

auto Pathname::isAbsolute( ) const -> bool {
    if ( path_ == "?" ) {
        throw std::runtime_error( "Invalid path" );
    }
    std::string::const_iterator first = path_.begin( );
    return ( '/' == *first );
}

auto Pathname::isRelative( ) const -> bool { return !isAbsolute( ); }

auto Pathname::toString( ) const -> const std::string & {
    if ( path_ == "?" ) {
        throw std::runtime_error( "Invalid path" );
    }
    return path_;
}

static std::string dirBaseSplit( const std::string path, const SplitType splitType ) {
    std::string::size_type slash_pos = path.rfind( '/' );
    if ( slash_pos != std::string::npos ) { // slash found
        switch ( splitType ) {
        case Basename:
            ++slash_pos;
            return ( slash_pos == path.size( ) ) ? "/" : path.substr( slash_pos );
        case Dirname:
            return ( 0 == slash_pos ) ? "/" : path.substr( 0, slash_pos );
        }
    } else {
        return path;
    }
}

