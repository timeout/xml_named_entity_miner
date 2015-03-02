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
    for ( int index = 0; index < count( ); ++index ) {
        TextDisplay *display = reinterpret_cast<TextDisplay *>( widget( index ) );
        delete display;
    }
    setCurrentIndex( -1 );
    emit enableNext( false );
    emit enablePrev( false );
}

void StackedTextDisplay::addElement( const XmlElement &element ) {
    // only add elements once
    auto search = std::find( std::begin( elements_ ), std::end( elements_ ), element );
    if ( search != std::end( elements_ ) ) {
        return;
    }

    auto iter = std::begin( elements_ );
    while ( iter != std::end( elements_ ) && *iter < element ) {
        ++iter;
    }
    pos_ = elements_.insert( iter, element );

    // create TextDisplay
    auto text = QString::fromStdString( element.content( ) );
    TextDisplay *textDisplay = new TextDisplay{text, this};
    connect( textDisplay, &TextDisplay::entrySelected, this,
             &StackedTextDisplay::entitySelected );

    // insert
    auto index = std::distance( std::begin( elements_ ), pos_ );
    insertWidget( index, textDisplay );

    setCurrentIndex( index );
    emit currentChanged( index );
    beginEnd( );
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
}

void StackedTextDisplay::addOntology( const QString &ontologyName,
                                      const QColor formatColor ) {
    qDebug( ) << "StackedTextDisplay#addOntology: " << ontologyName;
    ontologyMap_.emplace( ontologyName, formatColor );
}

void StackedTextDisplay::removeOntology( const QString &ontologyName ) {
    ontologyMap_.erase( ontologyName );
    // TODO:
    // remove ontologies in TextDisplay and TextDisplayHighlighter
}

void StackedTextDisplay::highlight( const QString &ontologyName, const QString &entity ) {
    qDebug( ) << "highlight: " << ontologyName << ", " << entity;
    for ( int i = 0; i < count( ); ++i ) {
        auto textDisplay = reinterpret_cast<TextDisplay *>( widget( i ) );
        // can throw an exception
        textDisplay->addHighlightRule( entity, ontologyMap_.at( ontologyName ) );
    }
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
