#include "xml_highlighter.hpp"
#include <QStringList>
#include <QRegExp>
#include <QColor>
#include <QDebug>

static const QRegExp commentStartExpr{"<!--"};
static const QRegExp commentEndExpr{"-->"};
static const QRegExp cdataStartExpr{"<!\\[CDATA\\["};
static const QRegExp cdataEndExpr{"\\]\\]>"};
static const QRegExp processStartExpr{"<\\?"};
static const QRegExp processEndExpr{"\\?>"};
static const QRegExp dtdStartExpr{"<!doctype", Qt::CaseInsensitive};
static const QRegExp dtdEndExpr{">"};

XmlHighlighter::XmlHighlighter( QTextDocument *parent ) : QSyntaxHighlighter{parent} {
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

void XmlHighlighter::setTagSet( const std::set<std::string> &tagSet ) {
    tags_.clear( );
    for ( auto tag : tagSet ) {
        QRegExp pattern{QString::fromUtf8( tag.c_str( ) )};
        tags_ << pattern;
    }
    entityReferences_ = {"&lt;", "&gt;", "&amp;", "&apos;", "&quot;"};
}

void XmlHighlighter::commentFormat( const QTextCharFormat &format ) {
    commentFormat_ = format;
}
void XmlHighlighter::cdataFormat( const QTextCharFormat &format ) {
    cdataFormat_ = format;
}
void XmlHighlighter::processFormat( const QTextCharFormat &format ) {
    processFormat_ = format;
}
void XmlHighlighter::dtdFormat( const QTextCharFormat &format ) { dtdFormat_ = format; }
void XmlHighlighter::tagFormat( const QTextCharFormat &format ) { tagFormat_ = format; }
void XmlHighlighter::enetityFormat( const QTextCharFormat &format ) {
    entityFormat_ = format;
}

bool XmlHighlighter::entityReference( const QString &entityReference ) const {
    return entityReferences_.contains( entityReference );
}
void XmlHighlighter::addEntityReference( const QString &entityReference ) {
    entityReferences_ << entityReference;
}
bool XmlHighlighter::removeEntityReference( const QString &entityReference ) {
    return entityReferences_.remove( entityReference );
}
void XmlHighlighter::highlightBlock( const QString &text ) {
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

// void XmlHighlighter::hlBlock( const QString &text, QRegExp startExpr, QRegExp endExpr,
//                               const QTextCharFormat &format,
//                               enum BlockState blockState ) {
void XmlHighlighter::hlBlock( const QString &text, const QRegExp &startExpr,
                              QRegExp endExpr, const QTextCharFormat &format,
                              enum BlockState blockState ) {
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

void XmlHighlighter::hlSet( const QString &text, const QRegExp &pattern,
                            const QTextCharFormat &format ) {
    int idx = pattern.indexIn( text );
    while ( idx >= 0 ) {
        int length = pattern.matchedLength( );
        setFormat( idx, length, format );
        idx = pattern.indexIn( text, idx + length );
    }
}

