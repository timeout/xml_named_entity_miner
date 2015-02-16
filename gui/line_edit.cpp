#include "line_edit.hpp"

#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>

LineEdit::LineEdit( QWidget *parent )
    : QLineEdit{parent}, activePalette_{palette( )},
      inactivePalette_{QGuiApplication::palette( )}, edited_{false} {
    inactivePalette_.setColor(
        QPalette::Active, QPalette::Text,
        inactivePalette_.color( QPalette::Disabled, QPalette::WindowText ) );
    inactivePalette_.setColor(
        QPalette::Inactive, QPalette::Text,
        inactivePalette_.color( QPalette::Disabled, QPalette::WindowText ) );
    setPalette( inactivePalette_ );
    connect( this, SIGNAL( keyPressed( bool )), SLOT( clearDefaultEntry( bool )) );
    connect( this, SIGNAL( mousePressed( bool )), SLOT( clearDefaultEntry( bool )) );
}

void LineEdit::clearDefaultEntry( bool ) {
    clear( );
    setPalette( activePalette_ );
}

void LineEdit::focusInEvent( QFocusEvent *event ) {
    QLineEdit::focusInEvent( event );
    emit( focussed( true ) );
}

void LineEdit::keyPressEvent( QKeyEvent *event ) {
    QLineEdit::keyPressEvent( event );
    if ( !edited_ ) {
        emit( keyPressed( true ) );
    }
    edited_ = true;
}

void LineEdit::mousePressEvent( QMouseEvent *event ) {
    QLineEdit::mousePressEvent( event );
    if ( !edited_ ) {
        emit( mousePressed( true ) );
    }
    edited_ = true;
}

