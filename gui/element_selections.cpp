#include "element_selections.hpp"
#include "xml_doc.hpp"
#include "utils.hpp"

#include <QtAlgorithms>
#include <algorithm>
#include <iostream>
#include <string>
#include <QDebug>

ElementSelections::ElementSelections( QObject *parent )
    : QObject{parent}, pos_{selections_.end( )} {}

void ElementSelections::clear( ) { selections_.clear( ); }

void ElementSelections::addSelection( const Selection &selection ) {
    // has the selection already been added?
    auto result =
        std::find( std::begin( selections_ ), std::end( selections_ ), selection );
    if ( result != std::end( selections_ ) ) {
        // if the selection does already exist, make it current and
        // return
        pos_ = result;
        emit currentIndexChanged( pos_ - selections_.begin( ) );
        return;
    }
    selections_.push_back( selection );
    std::sort( std::begin( selections_ ), std::end( selections_ ) );
    pos_ = std::find( std::begin( selections_ ), std::end( selections_ ), selection );

    emit selectionAdded( index( ), selection.text( ) );
    for ( int i = 0; i < selection.keywords( ).size( ); ++i ) {
        auto keywordList = selection.keywords( );
        for ( auto keyword : keywordList[i] ) {
            emit keywordAdded( index( ), i, keyword );
        }
    }
    emit currentIndexChanged( index( ) );
}

void ElementSelections::removeSelection( const XmlElement &element ) {
    auto search = std::find_if( std::begin( selections_ ), std::end( selections_ ),
                                [element]( Selection &selection ) {
        return ( selection.element( ) == element );
    } );
    if ( search == std::end( selections_ ) ) {
        return;
    }
    auto idx = index( );
    selections_.erase( search );
    // remove widget from stacked display
    emit selectionRemoved( search - std::begin( selections_ ) );
    if ( selections_.isEmpty( ) ) {
        emit clearSelections( );
        pos_ = std::end( selections_ );
        return;
    }
    pos_ = std::begin( selections_ );
    std::advance( pos_, idx );
    if ( pos_ == std::end( selections_ ) ) {
        --pos_;
    }
    emit currentIndexChanged( index( ) );
}

void ElementSelections::next( ) {
    if ( pos_ + 1 != selections_.end( ) ) {
        ++pos_;
        emit currentIndexChanged( index( ) );
    }
}

void ElementSelections::previous( ) {
    if ( pos_ != selections_.begin( ) ) {
        --pos_;
        emit currentIndexChanged( index( ) );
    }
}
