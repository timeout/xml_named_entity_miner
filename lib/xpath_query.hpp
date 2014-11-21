#pragma once
#include "xpath_ctxt.hpp"
#include <vector>

class FreeXPathQuery {
public:
    auto operator( )( xmlXPathObject *queryObject ) const -> void {
        xmlXPathFreeObject( queryObject );
    };
};

class XPathQuery {
public:
    XPathQuery( ) : query_{nullptr} {}
    explicit XPathQuery( const XPathCtxt &ctxt ) : query_{nullptr}, queryCtxt_{ctxt} {}
    XPathQuery( const XPathQuery & ) = delete;
    auto operator=( const XPathQuery & ) -> XPathQuery & = delete;
    friend auto operator>>( const XPathCtxt &xpctxt, XPathQuery &query ) -> XPathQuery & {
        query.queryCtxt_ = xpctxt;
        return query;
    }
    explicit operator bool( ) const { return ( query_ != nullptr ); }
    auto query( const std::string &xpath ) -> void {
        if ( queryCtxt_ ) {
            const xmlChar *expr = ( xmlChar * )xpath.c_str( );
            if ( query_ ) {
                query_.reset(
                    xmlXPathEvalExpression( expr, queryCtxt_.xpathCtxt_.get( ) ) );
            } else {
                query_ = std::move( XPathQueryT{
                    xmlXPathEvalExpression( expr, queryCtxt_.xpathCtxt_.get( ) )} );
            }
        }
    }

private:
    using XPathQueryT = std::unique_ptr<xmlXPathObject, FreeXPathQuery>;
    XPathQueryT query_;
    XPathCtxt queryCtxt_;
};
