#pragma once

#include "xpath_ctxt.hpp"

class FreeXPathQuery {
public:
    auto operator( )( xmlXPathObject *queryObject ) const -> void;
};

class XPathQuery {
    friend class XPathResultSet;

public:
    XPathQuery( );
    explicit XPathQuery( const XPathCtxt &ctxt );
    XPathQuery( const XPathQuery &query );
    XPathQuery( XPathQuery &&query );
    auto operator=( const XPathQuery &rhs ) -> XPathQuery & ;
    auto operator=( XPathQuery &&rhs ) -> XPathQuery &;
    friend auto operator>>( const XPathCtxt &xpctxt, XPathQuery &query ) -> XPathQuery &;
    explicit operator bool( ) const;
    auto query( const std::string &xpath ) -> void;
    // auto process( ) -> XPathResultSet {}

private:
    using XPathQueryT = std::unique_ptr<xmlXPathObject, FreeXPathQuery>;
    XPathQueryT query_;
    XPathCtxt queryCtxt_;
};

inline auto operator>>( const XPathCtxt &xpctxt, XPathQuery &query ) -> XPathQuery & {
    query.queryCtxt_ = xpctxt;
    return query;
}

