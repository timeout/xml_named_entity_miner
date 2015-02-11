#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <locale>

auto Utils::ltrim( std::string &str ) -> std::string & {
    auto it = str.begin( );
    for ( ; it != str.end( ); ++it ) {
        if ( !std::isblank( *it ) )
            break;
    }
    if ( it != str.begin( ) ) {
        str.erase( str.begin( ), it );
    }
    return str;
}
auto Utils::rtrim( std::string &str ) -> std::string & {
    auto it = str.rbegin( );
    for ( ; it != str.rend( ); ++it ) {
        if ( !std::isblank( *it ) )
            break;
    }
    if ( it != str.rbegin( ) ) {
        str.erase( it.base( ), str.end( ) );
    }
    return str;
}
auto Utils::trim( std::string &str ) -> std::string & { return rtrim( ltrim( str ) ); }
