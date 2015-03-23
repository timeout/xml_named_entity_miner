#include "xpath_result_set.hpp"
#include "libxml/xpath.h"

XPathResultSet::XPathResultSet( ) : query_{}, resultSet_{nullptr}, sz_{0}, idx_{0} {}

XPathResultSet::XPathResultSet( const XPathQuery &query ) : query_{query} {
    if ( query_ && query_.query_->nodesetval ) {
        resultSet_ = query_.query_->nodesetval;
        sz_ = resultSet_->nodeNr;
    } else {
        resultSet_ = nullptr;
        sz_ = 0;
    }
    idx_ = 0;
}

XPathResultSet::XPathResultSet( const XPathResultSet &other ) : query_{other.query_} {
    if ( query_ && query_.query_->nodesetval ) {
        resultSet_ = query_.query_->nodesetval;
        sz_ = resultSet_->nodeNr;
    } else {
        resultSet_ = nullptr;
        sz_ = 0;
    }
    idx_ = other.idx_;
}

XPathResultSet::XPathResultSet( XPathResultSet &&rhs )
    : query_{std::move( rhs.query_ )}
    , resultSet_{rhs.resultSet_}
    , sz_{rhs.sz_}
    , idx_{rhs.idx_} {}

auto XPathResultSet::operator=( const XPathResultSet &rhs ) -> XPathResultSet & {
    if ( this != &rhs ) {
        query_ = rhs.query_;
        if ( query_ && query_.query_->nodesetval ) {
            resultSet_ = query_.query_->nodesetval;
            sz_ = resultSet_->nodeNr;
        } else {
            resultSet_ = nullptr;
            sz_ = 0;
        }
        idx_ = rhs.idx_;
    }
    return *this;
}
auto XPathResultSet::operator=( XPathResultSet &&rhs ) -> XPathResultSet & {
    query_ = std::move( rhs.query_ );
    resultSet_ = rhs.resultSet_;
    sz_ = rhs.sz_;
    idx_ = rhs.idx_;
    return *this;
}

auto XPathResultSet::operator[]( size_t idx ) -> XmlElement {
    if ( resultSet_ && ( idx_ <= sz_ ) ) {
        idx_ = idx;
        return XmlElement{*( resultSet_->nodeTab + idx_ )};
    }
    return XmlElement{};
}

XPathResultSet::iterator::iterator( xmlNodeSet *nodeSet )
    : set_{nodeSet ? nodeSet->nodeTab : nullptr}
    , idx_{0}
    , size_{nodeSet ? static_cast<size_t>( nodeSet->nodeNr ) : 0} {}

auto XPathResultSet::iterator::operator*( ) -> XmlElement {
    return set_ ? XmlElement{*( set_ + idx_ )} : XmlElement{};
}

auto XPathResultSet::iterator::operator++( ) -> iterator & {

    ++idx_;
    if ( size_ == 0 || idx_ == size_ ) {
        idx_ = 0;
        set_ = nullptr;
    }
    return *this;
}

auto XPathResultSet::iterator::operator++( int ) -> iterator {
    iterator tmp = *this;
    ++idx_;
    if ( size_ == 0 || idx_ == size_ ) {
        idx_ = 0;
        set_ = nullptr;
    }
    return tmp;
}

auto XPathResultSet::iterator::operator--( ) -> iterator & {
    if ( size_ == 0 || idx_ == 0 ) {
        set_ = nullptr;
        return *this;
    }
    --idx_;
    return *this;
}

auto XPathResultSet::iterator::operator--( int ) -> iterator {
    if ( size_ == 0 || idx_ == 0 ) {
        set_ = nullptr;
        return *this;
    }
    iterator tmp = *this;
    --idx_;
    return tmp;
}

auto XPathResultSet::iterator::operator[]( std::size_t index ) -> XmlElement {
    if ( index >= size_ ) {
        idx_ = 0;
        set_ = nullptr;
    }
    idx_ = index;
    return set_ ? XmlElement{*( set_ + idx_ )} : XmlElement{};
}

auto XPathResultSet::iterator::operator==( const iterator &other ) const -> bool {
    return ( set_ == other.set_ ) && ( idx_ == other.idx_ );
}

auto XPathResultSet::iterator::operator!=( const iterator &other ) const -> bool {
    return ( set_ != other.set_ ) || ( idx_ != other.idx_ );
}

auto XPathResultSet::iterator::operator<( const iterator &other ) const -> bool {
    return ( set_ == other.set_ ) && ( idx_ < other.idx_ );
}

auto XPathResultSet::iterator::operator>( const iterator &other ) const -> bool {
    return ( set_ == other.set_ ) && ( idx_ > other.idx_ );
}

auto XPathResultSet::iterator::operator<=( const iterator &other ) const -> bool {
    return ( set_ == other.set_ ) && ( idx_ <= other.idx_ );
}

auto XPathResultSet::iterator::operator>=( const iterator &other ) const -> bool {
    return ( set_ == other.set_ ) && ( idx_ >= other.idx_ );
}

