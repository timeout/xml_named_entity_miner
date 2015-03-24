#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <locale>
#include <cctype>
#include <iterator>
#include <algorithm>
#include <functional>

#include <iostream>

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
auto Utils::collapseWhiteSpace( std::string::iterator start, std::string::iterator end )
    -> std::string::iterator {
    for ( ; start != end; ++start ) {
        if ( std::isspace( *start ) && std::isspace( *std::next( start ) ) ) {
            auto white = std::next( start );
            auto nonwhite = std::find_if(
                white, end, [=]( char c ) -> bool { return !std::isspace( c ); } );
            if ( nonwhite == end ) {
                return start;
            }
            while ( nonwhite != end && !std::isspace( *nonwhite ) ) {
                std::swap( *white++, *nonwhite );
                ++nonwhite;
            }
            start = std::next( white, -1 );
        }
    }
    return end;
}
// auto Utils::sentenceBoundary( const std::string &text, std::string::iterator start,
//                               std::string::iterator end ) -> std::string::iterator {}

