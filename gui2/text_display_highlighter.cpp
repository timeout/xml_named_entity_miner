#include "text_display_highlighter.hpp"

#include <QDebug>

TextDisplayHighlighter::TextDisplayHighlighter( QTextDocument *document )
    : QSyntaxHighlighter{document} {}

auto TextDisplayHighlighter::addRule( const QString &rule, const QColor &color ) -> void {
    qDebug( ) << "adding rule: " << rule << " with color: " << color;
    QTextCharFormat format;
    // TODO: formatting
    format.setForeground( color );
    format.setBackground( QColor{0xee, 0xe8, 0xd5} );
    rules_.insert( rule, format );
    rehighlight( );
}

auto TextDisplayHighlighter::removeRule( const QString &rule ) -> void {
    rules_.remove( rule );
}

auto TextDisplayHighlighter::removeAll( const QColor &color ) -> void {
    for ( auto iter = rules_.begin( ); iter != rules_.end( ); ) {
        auto format = *iter;
        if ( format.foreground( ).color( ) == color ) {
            iter = rules_.erase( iter );
        } else {
            ++iter;
        }
    }
}

auto TextDisplayHighlighter::highlightBlock( const QString &text ) -> void {
    for ( auto iter = rules_.begin( ); iter != rules_.end( ); ++iter ) {
        QRegExp expr( iter.key( ) );
        int index = expr.indexIn( text );
        while ( index >= 0 ) {
            int length = expr.matchedLength( );
            setFormat( index, length, *iter );
            index = expr.indexIn( text, index + length );
        }
    }
}
