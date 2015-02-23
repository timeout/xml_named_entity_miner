#include "text_display.hpp"

TextDisplay::TextDisplay( QWidget *parent ) : QPlainTextEdit{parent} {}

TextDisplay::TextDisplay( const QString &text, QWidget *parent )
    : QPlainTextEdit{parent} {
    setPlainText( text );
    highlighter_ = new TextDisplayHighlighter( document( ) );
}

TextDisplay::~TextDisplay( ) {}

auto TextDisplay::addOntology( const QColor &color ) -> void {
    ontologyVec_.push_back( color );
}

auto TextDisplay::removeOntology( int ontologyIndex ) -> void {
    auto pos = ontologyVec_.begin( ) + ontologyIndex;
    auto color = *pos;
    ontologyVec_.erase( pos );
    highlighter_->removeAll( color );
}

auto TextDisplay::addHighlightRule( int ontologyIndex, const QString &rule ) -> void {
    auto color = ontologyVec_.at( ontologyIndex );
    highlighter_->addRule( rule, color );
}

auto TextDisplay::removeHighlightRule( int ontologyIndex, const QString &rule ) -> void {
    highlighter_->removeRule( rule );
}

void TextDisplay::mousePressEvent( QMouseEvent *event ) {
    if ( event->button( ) == Qt::LeftButton ) {
        QPoint startPos = event->pos( );
        cursorSelection_.begin_ = cursorForPosition( startPos );
    }
    QPlainTextEdit::mousePressEvent( event );
}

void TextDisplay::mouseReleaseEvent( QMouseEvent *event ) {
    if ( event->button( ) == Qt::LeftButton ) {
        QPoint endPos = event->pos( );
        cursorSelection_.end_ = cursorForPosition( endPos );
        cursorSelection( );
    }
    QPlainTextEdit::mouseReleaseEvent( event );
}

auto TextDisplay::cursorSelection( ) -> void {
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

    emit entrySelected( textSelection );
}

