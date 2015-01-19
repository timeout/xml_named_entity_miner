#include "element_selections.hpp"
#include "xml_doc.hpp"

#include <iostream>

ElementSelections::ElementSelections( QObject *parent ) : QObject{parent}, num_{0} {
    idx_ = selections_.begin( );
}

void ElementSelections::clear( ) {
    selections_.clear( );
    idx_ = selections_.begin( );
    num_ = 0;
    emit currentXmlElementInvalid( );
}
void ElementSelections::addXmlElement( const XmlElement &element ) {
    if ( selections_.empty( ) ) {
        idx_ = selections_.begin( );
        num_ = 1;
    }
    auto result =
        std::find( std::begin( selections_ ), std::end( selections_ ), element );
    if ( result != std::end( selections_ ) ) {
        idx_ = result;
        currentXmlElement( *idx_ );
        return;
    }
    selections_.push_back( element );
    selections_.sort( );
    emit currentXmlElement( selections_.front( ) );
    emit currentIdx( num_ );
}
void ElementSelections::removeXmlElement( const XmlElement &element ) {
    std::list<XmlElement>::iterator fpos =
        std::find( selections_.begin( ), selections_.end( ), element );
    if ( fpos != std::end( selections_ ) ) {
        if ( fpos == idx_ ) {
            if ( selections_.size( ) > 1 ) {
                auto next = std::next( idx_, 1 );
                if ( next != selections_.end( ) ) {
                    idx_ = next;
                    emit currentXmlElement( *idx_ );
                } else {
                    next = std::prev( idx_, 1 );
                    idx_ = next;
                    --num_;
                    emit currentXmlElement( *idx_ );
                }
            } else {
                idx_ = selections_.begin( );
                num_ = 0;
                emit currentXmlElementInvalid( );
            }
        }
        emit currentIdx( num_ );
        selections_.erase( fpos );
    }
}
void ElementSelections::next( ) {
    if ( !selections_.empty( ) ) {
        ++idx_;
        ++num_;
        if ( idx_ == selections_.end( ) ) {
            idx_ = selections_.begin( );
            num_ = 1;
        }
        emit currentXmlElement( *idx_ );
    }
    emit currentIdx( num_ );
}
void ElementSelections::previous( ) {
    if ( !selections_.empty( ) ) {
        if ( idx_ == selections_.begin( ) ) {
            idx_ = selections_.end( );
            num_ = selections_.size( ) + 1;
        }
        --idx_;
        --num_;
        emit currentXmlElement( *idx_ );
    }
    emit currentIdx( num_ );
}
