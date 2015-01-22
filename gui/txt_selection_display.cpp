#include "txt_selection_display.hpp"
#include "xml_element.hpp"
#include <QString>
#include <QPalette>
#include <QGuiApplication>

#include <iostream>

TxtSelectionDisplay::TxtSelectionDisplay( QWidget *parent ) : QPlainTextEdit( parent ) {}

void TxtSelectionDisplay::setXmlTxt( const XmlElement &element ) {
    auto txt = element.content( );
    setPlainText( QString::fromUtf8( txt.c_str( ) ) );
}

void TxtSelectionDisplay::mousePressEvent( QMouseEvent *event ) {
    if ( event->button( ) == Qt::LeftButton ) {
        QPoint startPos = event->pos( );
        cursorSelection_.begin_ = cursorForPosition( startPos );
    }
    QPlainTextEdit::mousePressEvent( event );
}
void TxtSelectionDisplay::mouseReleaseEvent( QMouseEvent *event ) {
    if ( event->button( ) == Qt::LeftButton ) {
        QPoint endPos = event->pos( );
        cursorSelection_.end_ = cursorForPosition( endPos );
        cursorSelection( );
    }
    QPlainTextEdit::mouseReleaseEvent( event );
}

void TxtSelectionDisplay::cursorSelection( ) {
    if ( cursorSelection_.begin_.blockNumber( ) !=
         cursorSelection_.end_.blockNumber( ) ) {
        return;
    }
    if ( cursorSelection_.begin_.position( ) > cursorSelection_.end_.position( ) ) {
        std::swap( cursorSelection_.begin_, cursorSelection_.end_ );
    }
    QTextEdit::ExtraSelection selection;
    if ( cursorSelection_.begin_ == cursorSelection_.end_ ) {
        selection.cursor = cursorSelection_.begin_;
        selection.cursor.select( QTextCursor::WordUnderCursor );
    } else {
        cursorSelection_.begin_.movePosition( QTextCursor::StartOfWord );
        cursorSelection_.end_.movePosition( QTextCursor::EndOfWord );
        selection.cursor = textCursor( );
        selection.cursor.setPosition( cursorSelection_.begin_.position( ) );
        selection.cursor.setPosition( cursorSelection_.end_.position( ),
                                      QTextCursor::KeepAnchor );
    }
    QString textSelection = selection.cursor.selectedText( );

    // highlight all occurrences of text selection
    // find occurrences, add to extraSelections and call
    // QPlainTextEdit#setExtraSelections()
    extraSelections_ << findAll( textSelection );
    setExtraSelections( extraSelections_ );
}

const QList<QTextEdit::ExtraSelection>
TxtSelectionDisplay::findAll( const QString &textSelection ) {
    QList<QTextEdit::ExtraSelection> ret;
    moveCursor( QTextCursor::Start );
    while ( find( textSelection ) ) {
        QTextEdit::ExtraSelection selection;
        QPalette palette = QGuiApplication::palette( );
        selection.format.setBackground( palette.background( ) );
        selection.format.setForeground( palette.highlightedText( ) );
        selection.cursor = textCursor( );
        ret.append( selection );
    }
    return ret;
}


