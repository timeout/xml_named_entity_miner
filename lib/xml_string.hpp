#pragma once
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
#include <memory>
#include <string>
#include <iostream>

class FreeXmlString {
public:
    auto operator( )( xmlChar *ustr ) const -> void { xmlFree( ustr ); }
};

class XmlString {
public:
    XmlString( ) : ustr_{nullptr} {}
    XmlString( xmlChar *ustr ) : ustr_{ustr} {}
    XmlString( const XmlString &str )
        : ustr_{xmlCharStrdup( reinterpret_cast<char *>( str.ustr_.get( ) ) )} {}
    XmlString( XmlString &&str ) : ustr_{std::move( str.ustr_ )} {}
    auto operator=( const XmlString &rhs ) -> XmlString & {
        if ( this != &rhs ) {
            ustr_.reset( xmlCharStrdup( reinterpret_cast<char *>( rhs.ustr_.get( ) ) ) );
        }
        return *this;
    }
    auto operator=( XmlString &&rhs ) -> XmlString & {
        ustr_ = std::move( rhs.ustr_ );
        return *this;
    }
    auto toString( ) const -> std::string {
        std::string ret{reinterpret_cast<char *>( ustr_.get( ) )};
        if ( ret.back( ) == '\n' ) { // libxml2 adds a newline character?
            ret.pop_back( );
        }
        return ret;
    }

private:
    using XmlStringT = std::unique_ptr<xmlChar, FreeXmlString>;
    XmlStringT ustr_;
};

inline auto operator<<( std::ostream &os, const XmlString &str ) -> std::ostream & {
    os << str.toString( );
    return os;
}
