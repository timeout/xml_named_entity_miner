#pragma once
#include "xml_display.hpp"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QSet>
#include <QVector>
#include <set>
#include <string>

// What to highlight?
// 1. comments
// 2. CDATA
// 3. Processing instructions
// 4. DTD
// 5. tags
// 6. entity references

enum BlockState {
    NO_STATE = 0x0,
    COMMENT_STATE = 0x1,
    CDATA_STATE = 0x2,
    PROCESS_STATE = 0x4,
    DTD_STATE = 0x8,
    INTERNAL_DTD = 0x10
};

class XmlHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    XmlHighlighter( QTextDocument *parent = nullptr );

    // clears and sets
    void setTagSet( const std::set<std::string> &tagSet );

    void commentFormat( const QTextCharFormat &format );
    void cdataFormat( const QTextCharFormat &format );
    void processFormat( const QTextCharFormat &format );
    void dtdFormat( const QTextCharFormat &format );
    void tagFormat( const QTextCharFormat &format );
    void enetityFormat( const QTextCharFormat &format );

    // default initialized in ctor, "&lt;" etc...
    bool entityReference( const QString &entityReference ) const;
    void addEntityReference( const QString &entityReference );
    bool removeEntityReference( const QString &entityReference );

protected:
    void highlightBlock( const QString &text ) override;

private:
    void hlBlock( const QString &text, const QRegExp &startExpr, QRegExp endExpr,
                  const QTextCharFormat &format, enum BlockState blockState );
    void hlSet( const QString &text, const QRegExp &pattern,
                const QTextCharFormat &format );

    QVector<QRegExp> tags_;
    QSet<QString> entityReferences_;

    QTextCharFormat commentFormat_;
    QTextCharFormat cdataFormat_;
    QTextCharFormat processFormat_;
    QTextCharFormat dtdFormat_;
    QTextCharFormat tagFormat_;
    QTextCharFormat entityFormat_;
};
