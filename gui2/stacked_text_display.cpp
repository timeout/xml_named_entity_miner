#include "stacked_text_display.hpp"
#include "text_display.hpp"

#include <iostream>

StackedTextDisplay::StackedTextDisplay( QWidget *parent ) : QStackedWidget{parent} {}

void StackedTextDisplay::clear( ) {
    std::cerr << "clearing stacked text display" << std::endl;
    for ( int index = 0; index < count( ); ++index ) {
        TextDisplay *display = reinterpret_cast<TextDisplay *>( widget( index ) );
        delete display;
    }
}

void StackedTextDisplay::insertDisplay( int index, const QString &text ) {
    TextDisplay *display = new TextDisplay( text );
    // connect( display, &TextDisplay::entrySelected, &StackedTextDisplay::addDisplayIndex);
    insertWidget( index, display );
}

void StackedTextDisplay::removeDisplay( int index ) {
    std::cerr << "removing display: " << index << std::endl;
    TextDisplay *display = reinterpret_cast<TextDisplay *>( widget( index ) );
    delete display;
    std::cerr << "display count: " << count( ) << std::endl;
}

/// adds ontology at index index to all displays
void StackedTextDisplay::addOntology( const QColor &color ) {
    for ( int index = 0; index < count( ); ++index ) {
        TextDisplay *display = reinterpret_cast<TextDisplay *>( widget( index ) );
        display->addOntology( color );
    }
}

void StackedTextDisplay::removeOntology( int ontologyIndex ) {
    for ( int index = 0; index < count( ); ++index ) {
        TextDisplay *display = reinterpret_cast<TextDisplay *>( widget( index ) );
        display->removeOntology( ontologyIndex );
    }
}

/// add rule to widget at index  to ontology rule set at index ontologyIndex
void StackedTextDisplay::insertHighlightRule( int index, int ontologyIndex,
                                              const QString &rule ) {
    TextDisplay *display = reinterpret_cast<TextDisplay *>( widget( index ) );
    display->addHighlightRule( ontologyIndex, rule );
}

void StackedTextDisplay::removeHighlightRule( int index, int ontologyIndex,
                                              const QString &rule ) {
    TextDisplay *display = reinterpret_cast<TextDisplay *>( widget( index ) );
    display->removeHighlightRule( ontologyIndex, rule );
}

void StackedTextDisplay::addDisplayIndex( const QString &entry ) {
    
}
