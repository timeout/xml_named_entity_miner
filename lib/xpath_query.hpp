#pragma once

#include "xpath_ctxt.hpp"
// #include "xpath_result_set.hpp"

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
    XPathQuery( const XPathQuery &query ) : queryCtxt_{query.queryCtxt_} {
        if ( query.query_ ) {
            query_.reset( xmlXPathObjectCopy( query.query_.get( ) ) );
        } else {
            query_.reset( nullptr );
        }
    }
    XPathQuery( XPathQuery &&query )
        : query_{std::move( query.query_ )}, queryCtxt_{std::move( query.queryCtxt_ )} {}
    auto operator=( const XPathQuery &rhs ) -> XPathQuery & {
        if ( this != &rhs ) {
            queryCtxt_ = rhs.queryCtxt_;
            if ( rhs.query_ ) {
                query_.reset( xmlXPathObjectCopy( rhs.query_.get( ) ) );
            } else {
                query_.reset( nullptr );
            }
        }
        return *this;
    }
    auto operator=( XPathQuery &&rhs ) -> XPathQuery & {
        query_ = std::move( rhs.query_ );
        queryCtxt_ = std::move( rhs.queryCtxt_ );
        return *this;
    }
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
                // query_ = std::move( XPathQueryT{
                //     xmlXPathEvalExpression( expr, queryCtxt_.xpathCtxt_.get( ) )} );
                XPathQueryT tmp{
                    xmlXPathEvalExpression( expr, queryCtxt_.xpathCtxt_.get( ) )};
                query_.swap( tmp );
            }
        }
    }
    // auto process( ) -> XPathResultSet {}

private:
    using XPathQueryT = std::unique_ptr<xmlXPathObject, FreeXPathQuery>;
    XPathQueryT query_;
    XPathCtxt queryCtxt_;
};
