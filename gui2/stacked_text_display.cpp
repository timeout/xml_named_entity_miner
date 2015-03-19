#include "stacked_text_display.hpp"
#include "text_display.hpp"

#include <algorithm>
#include <iostream>

#include <QDebug>

StackedTextDisplay::StackedTextDisplay( QWidget *parent ) : QStackedWidget{parent} {
    pos_ = elements_.begin( );
    connections( );
}

void StackedTextDisplay::clear( ) {
    std::cerr << "clearing stacked text display" << std::endl;
    auto size = count( );
    for ( int index = 0; index < size; ++index ) {
        TextDisplay *display = reinterpret_cast<TextDisplay *>( widget( index ) );
        delete display;
        emit widgetRemoved( index );
    }
    setCurrentIndex( -1 );
    elements_.clear( );
    emit enableNext( false );
    emit enablePrev( false );
    emit hasContent( false );
}

void StackedTextDisplay::addElement( const XmlElement &element ) {
    // only add elements once
    auto search = std::find( std::begin( elements_ ), std::end( elements_ ), element );
    if ( search != std::end( elements_ ) ) {
        return;
    }

    std::cerr << "adding element, elements current size: " << elements_.size( )
              << std::endl;

    auto iter = std::begin( elements_ );
    while ( iter != std::end( elements_ ) && *iter < element ) {
        ++iter;
    }
    pos_ = elements_.insert( iter, element );

    // create TextDisplay
    TextDisplay *textDisplay = new TextDisplay{element, this};
    connect( textDisplay, &TextDisplay::entrySelected, this,
             &StackedTextDisplay::entitySelected );

    // insert
    auto index = std::distance( std::begin( elements_ ), pos_ );
    insertWidget( index, textDisplay );

    setCurrentIndex( index );
    emit currentChanged( index );
    emit textAdded( textDisplay->text( ) );
    beginEnd( );

    emit hasContent( true );
}

void StackedTextDisplay::removeElement( const XmlElement &element ) {
    auto search = std::find( std::begin( elements_ ), std::end( elements_ ), element );
    if ( search == std::end( elements_ ) ) {
        return;
    }
    auto index = std::distance( std::begin( elements_ ), search );
    TextDisplay *textDisplay = reinterpret_cast<TextDisplay *>( widget( index ) );
    delete textDisplay;
    elements_.erase( search );
    emit widgetRemoved( index );
    if ( count( ) == 0 ) { // there _was_ only one display
        setCurrentIndex( -1 );
        pos_ = std::begin( elements_ );
    } else if ( index == 0 ) { // removed first display
        setCurrentIndex( 0 );
        pos_ = std::begin( elements_ );
    } else if ( index == count( ) ) { // removed last display
        qDebug( ) << "deleting last display";
        setCurrentIndex( --index );
        pos_ = std::prev( std::end( elements_ ), 1 );
    } else {
        setCurrentIndex( index );
    }
    beginEnd( );
    emit currentChanged( index );

    if ( count( ) <= 0 ) {
        qDebug( ) << "has content: false";
        emit hasContent( false );
    }
}

void StackedTextDisplay::addOntology( const QString &ontologyName,
                                      const QColor formatColor ) {
    qDebug( ) << "StackedTextDisplay#addOntology: " << ontologyName;
    ontologyMap_.emplace( ontologyName, formatColor );
}

void StackedTextDisplay::removeOntology( const QString &ontologyName ) {
    for ( auto i = count( ); i; ) {
        qDebug( ) << "index i: " << i;
        auto textDisplay = reinterpret_cast<TextDisplay *>( widget( --i ) );
        qDebug( ) << "removing all highlight rules for: " << ontologyName << " ( "
                  << ontologyMap_.at( ontologyName ) << " )";
        textDisplay->removeAllHighlightRules( ontologyMap_.at( ontologyName ) );
    }
    ontologyMap_.erase( ontologyName );
}

void StackedTextDisplay::highlight( const QString &ontologyName, const QString &entity ) {
    qDebug( ) << "highlight: " << ontologyName << ", " << entity;
    for ( int i = 0; i < count( ); ++i ) {
        auto textDisplay = reinterpret_cast<TextDisplay *>( widget( i ) );
        std::cerr << textDisplay->text( ).toStdString( ) << std::endl;
        std::cerr << "--------------------" << std::endl;
        // can throw an exception
        // textDisplay->addHighlightRule( entity, ontologyMap_.at( ontologyName ) );
        textDisplay->scanKeyword( ontologyName, entity, ontologyMap_.at( ontologyName ) );
    }
    setWindowModified( true );
}

void StackedTextDisplay::removeHighlight( const QString &entity ) {
    for ( int i = 0; i < count( ); ++i ) {
        auto textDisplay = reinterpret_cast<TextDisplay *>( widget( i ) );
        textDisplay->removeHighlightRule( entity );
    }
}

void StackedTextDisplay::next( ) {
    auto it = pos_;
    if ( std::next( it ) != std::end( elements_ ) ) {
        ++pos_;
    }
    beginEnd( );
    auto index = std::distance( std::begin( elements_ ), pos_ );
    setCurrentIndex( index );
    emit currentChanged( index );
}

void StackedTextDisplay::prev( ) {
    if ( pos_ != std::begin( elements_ ) ) {
        --pos_;
    }
    beginEnd( );
    auto index = std::distance( std::begin( elements_ ), pos_ );
    setCurrentIndex( index );
    emit currentChanged( index );
}

void StackedTextDisplay::unlock( ) {
    TextDisplay *textDisplay =
        reinterpret_cast<TextDisplay *>( widget( currentIndex( ) ) );
    textDisplay->unlock( );
    // disable next and prev
    emit enableNext( false );
    emit enablePrev( false );
    emit disabled( true );
}

void StackedTextDisplay::lock( ) {
    TextDisplay *textDisplay =
        reinterpret_cast<TextDisplay *>( widget( currentIndex( ) ) );
    textDisplay->lock( );
    // enable next and prev
    beginEnd( );
    emit disabled( false );
}

auto StackedTextDisplay::currentTextDisplayLocked( ) const -> bool {
    const TextDisplay *textDisplay =
        reinterpret_cast<TextDisplay *>( widget( currentIndex( ) ) );
    return textDisplay->isLocked( );
}

auto StackedTextDisplay::beginEnd( ) -> void {
    if ( pos_ == std::begin( elements_ ) ) {
        emit enablePrev( false );
    } else {
        emit enablePrev( true );
    }
    auto it = pos_;
    if ( pos_ == std::end( elements_ ) || std::next( it ) == std::end( elements_ ) ) {
        emit enableNext( false );
    } else {
        emit enableNext( true );
    }
}

auto StackedTextDisplay::connections( ) -> void {
    auto textDisplay = reinterpret_cast<TextDisplay *>( currentWidget( ) );
    connect( textDisplay, &TextDisplay::entrySelected, this,
             &StackedTextDisplay::entitySelected );
}
