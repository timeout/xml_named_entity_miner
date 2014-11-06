#pragma once

#include <string>
#include <iostream>

class Pathname {
public:
    Pathname( );
    Pathname( const std::string &path );
    auto isValid() const -> bool;
    auto basename( ) const -> Pathname;
    auto dirname( ) const -> Pathname;
    auto isAbsolute( ) const -> bool;
    auto isRelative( ) const -> bool;
    auto toString( ) const -> const std::string &;
    auto operator+=( const Pathname &path ) -> Pathname &;
    friend auto operator>>( std::istream &is, const Pathname &pathname ) -> std::istream
        &;
    friend auto operator+( Pathname lhs, const Pathname &rhs ) -> Pathname;
    friend auto operator==( const Pathname &lhs, const Pathname &rhs ) -> bool;
    friend auto operator!=( const Pathname &lhs, const Pathname &rhs ) -> bool;
    friend auto operator<( const Pathname &lhs, const Pathname &rhs ) -> bool;
    friend auto operator>( const Pathname &lhs, const Pathname &rhs ) -> bool;
    friend auto operator<=( const Pathname &lhs, const Pathname &rhs ) -> bool;
    friend auto operator>=( const Pathname &lhs, const Pathname &rhs ) -> bool;

private:
    std::string path_;
};

inline std::ostream &operator<<( std::ostream &os, const Pathname &pathname ) {
    os << pathname.toString( );
    return os;
}

inline std::istream &operator>>( std::istream &is, const Pathname &pathname ) {
    is >> pathname.path_;
    return is;
}

inline auto operator+( Pathname lhs, const Pathname &rhs ) -> Pathname {
    return lhs += rhs;
}

inline auto operator==( const Pathname &lhs, const Pathname &rhs ) -> bool {
    return ( lhs.path_ == rhs.path_ );
}

inline auto operator!=( const Pathname &lhs, const Pathname &rhs ) -> bool {
    return ( lhs.path_ != rhs.path_ );
}

inline auto operator<( const Pathname &lhs, const Pathname &rhs ) -> bool {
    return ( lhs.path_ < rhs.path_ );
}

inline auto operator>( const Pathname &lhs, const Pathname &rhs ) -> bool {
    return rhs < lhs;
}

inline auto operator<=( const Pathname &lhs, const Pathname &rhs ) -> bool {
    return !( lhs > rhs );
}

inline auto operator>=( const Pathname &lhs, const Pathname &rhs ) -> bool {
    return !( lhs < rhs );
}

