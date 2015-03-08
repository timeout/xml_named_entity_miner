#include "text_display.hpp"
#include <QDebug>

TextDisplay::TextDisplay( const QString &text, QWidget *parent )
    : QPlainTextEdit{parent}, lock_{true} {
    setReadOnly( true ); // lock_
    setPlainText( text );
    highlighter_ = new TextDisplayHighlighter( document( ) );
    connections( );
}

TextDisplay::~TextDisplay( ) {}

auto TextDisplay::addHighlightRule( const QString &entity, const QColor &color ) -> void {
    highlighter_->addRule( entity, color );
}

auto TextDisplay::removeHighlightRule( const QString &entity ) -> void {
    highlighter_->removeRule( entity );
}

auto TextDisplay::removeAllHighlightRules( const QColor &color ) -> void {
    qDebug() << "removing all rules associated with the color: " << color;
    highlighter_->removeAll( color );
}

auto TextDisplay::unlock( ) -> void {
    qDebug( ) << "unlocking text display";
    lock_ = false;
    setReadOnly( lock_ );
    // could be edits
    // caught via signal QPlainTextEdit#modificationChanged(bool)
}

auto TextDisplay::lock( ) -> void {
    qDebug( ) << "locking text display";
    lock_ = true;
    setReadOnly( lock_ );
}

auto TextDisplay::isLocked( ) const -> bool { return lock_; }

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
    if ( lock_ == false ) {
        qDebug( ) << "Text edit locked, cursor selection disabled";
        return;
    }
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
    if ( textSelection.isEmpty( ) ) {
        return;
    }

    qDebug( ) << "text selection: " << textSelection;
    emit entrySelected( textSelection );
}

auto TextDisplay::connections( ) -> void {
    connect( this, &TextDisplay::modificationChanged, highlighter_,
             &TextDisplayHighlighter::rehighlightModification );
}

