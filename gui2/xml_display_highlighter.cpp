#include "xml_display_highlighter.hpp"
#include "xml_doc.hpp"
#include "xml_element.hpp"
#include <queue>
#include <QColor>

static const QRegExp commentStartExpr{"<!--"};
static const QRegExp commentEndExpr{"-->"};
static const QRegExp cdataStartExpr{"<!\\[CDATA\\["};
static const QRegExp cdataEndExpr{"\\]\\]>"};
static const QRegExp processStartExpr{"<\\?"};
static const QRegExp processEndExpr{"\\?>"};
static const QRegExp dtdStartExpr{"<!doctype", Qt::CaseInsensitive};
static const QRegExp dtdEndExpr{">"};

XmlDisplayHighlighter::XmlDisplayHighlighter( QTextDocument *parent )
    : QSyntaxHighlighter{parent} {
    // default format styles
    commentFormat_.setForeground( QColor{0x83, 0x94, 0x96} ); // base0
    commentFormat_.setFontItalic( true );

    cdataFormat_.setForeground( QColor{0x58, 0x6e, 0x75} ); // base01
    cdataFormat_.setBackground( QColor{0xee, 0xe8, 0xd5} ); // base 2

    processFormat_.setForeground( QColor{0x26, 0x8b, 0xd2} ); // blue
    processFormat_.setBackground( QColor{0xee, 0xe8, 0xd5} ); // base 2

    dtdFormat_.setForeground( QColor{0x58, 0x6e, 0x75} ); // base01
    dtdFormat_.setBackground( QColor{0xee, 0xe8, 0xd5} ); // base 2

    tagFormat_.setForeground( QColor{0x6c, 0x71, 0xc4} ); // violet
    tagFormat_.setFontWeight( QFont::DemiBold );

    entityFormat_.setForeground( QColor{0x83, 0x94, 0x96} ); // base01
    entityFormat_.setBackground( QColor{0xee, 0xe8, 0xd5} ); // base 2
}

auto XmlDisplayHighlighter::generateTagSet( const XmlDoc &xml ) -> void {
    tags_.clear( );
    std::queue<XmlElement> elementQueue;
    XmlElement root = xml.getRootElement( );
    elementQueue.push( root );
    while ( !elementQueue.empty( ) ) {
        XmlElement parent{elementQueue.front( )};
        elementQueue.pop( );
        QRegExp regex{QString::fromStdString( parent.tagsRegex( ).first )};
        tags_ << regex;
        if ( !parent.tagsRegex( ).second.empty( ) ) {
            QRegExp regex{QString::fromStdString( parent.tagsRegex( ).second )};
            tags_ << regex;
        }
        for ( auto child : parent.children( ) ) {
            elementQueue.push( child );
        }
    }
    entityReferences_ = {"&lt;", "&gt;", "&amp;", "&apos;", "&quot;"};
}

auto XmlDisplayHighlighter::commentFormat( const QTextCharFormat &format ) -> void {
    commentFormat_ = format;
}

auto XmlDisplayHighlighter::cdataFormat( const QTextCharFormat &format ) -> void {
    cdataFormat_ = format;
}

auto XmlDisplayHighlighter::processFormat( const QTextCharFormat &format )  -> void{
    processFormat_ = format;
}

auto XmlDisplayHighlighter::dtdFormat( const QTextCharFormat &format ) -> void {
    dtdFormat_ = format;
}

auto XmlDisplayHighlighter::tagFormat( const QTextCharFormat &format ) -> void {
    tagFormat_ = format;
}

auto XmlDisplayHighlighter::enetityFormat( const QTextCharFormat &format ) -> void {
    entityFormat_ = format;
}

auto XmlDisplayHighlighter::entityReference( const QString &entityReference ) const ->bool {
    return entityReferences_.contains( entityReference );
}

auto XmlDisplayHighlighter::addEntityReference( const QString &entityReference ) -> void {
    entityReferences_ << entityReference;
}

auto XmlDisplayHighlighter::removeEntityReference( const QString &entityReference ) -> bool{
    return entityReferences_.remove( entityReference );
}

auto XmlDisplayHighlighter::highlightBlock( const QString &text ) -> void {
    foreach ( const QRegExp &pattern, tags_ ) { hlSet( text, pattern, tagFormat_ ); }
    foreach ( const QString &entityReference, entityReferences_ ) {
        QRegExp pattern = QRegExp( entityReference );
        hlSet( text, pattern, entityFormat_ );
    }
    hlBlock( text, commentStartExpr, commentEndExpr, commentFormat_,
             BlockState::COMMENT_STATE );
    hlBlock( text, cdataStartExpr, cdataEndExpr, cdataFormat_, BlockState::CDATA_STATE );
    hlBlock( text, processStartExpr, processEndExpr, processFormat_,
             BlockState::PROCESS_STATE );
    hlBlock( text, dtdStartExpr, dtdEndExpr, dtdFormat_, BlockState::DTD_STATE );
}

auto XmlDisplayHighlighter::hlBlock( const QString &text, const QRegExp &startExpr,
                                     QRegExp endExpr, const QTextCharFormat &format,
                                     enum BlockState blockState ) -> void {
    setCurrentBlockState( BlockState::NO_STATE );

    int startIdx{0};
    if ( previousBlockState( ) != blockState ) { // define 1 as in block
        startIdx = startExpr.indexIn( text );
    }

    while ( startIdx >= 0 ) {
        if ( blockState == BlockState::DTD_STATE &&
             QRegExp{"\\["}.indexIn( text, startIdx ) > 0 ) {
            setCurrentBlockState( BlockState::INTERNAL_DTD );
            endExpr = QRegExp{"\\]>"};
        }
        if ( previousBlockState( ) == BlockState::DTD_STATE ) {
            endExpr = QRegExp{"\\]>"};
        }
        int endIdx{endExpr.indexIn( text, startIdx )};
        int length;
        if ( endIdx == -1 ) {
            setCurrentBlockState( blockState );
            length = text.length( ) - startIdx;
        } else {
            length = endIdx - startIdx + endExpr.matchedLength( );
        }

        setFormat( startIdx, length, format );
        startIdx = startExpr.indexIn( text, startIdx + length );
    }
}

auto XmlDisplayHighlighter::hlSet( const QString &text, const QRegExp &pattern,
                                   const QTextCharFormat &format ) -> void {
    int idx = pattern.indexIn( text );
    while ( idx >= 0 ) {
        int length = pattern.matchedLength( );
        setFormat( idx, length, format );
        idx = pattern.indexIn( text, idx + length );
    }
}

