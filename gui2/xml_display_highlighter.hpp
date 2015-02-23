#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegExp>
#include <QSet>
#include <QString>
#include <QVector>

// What to highlight?
// 1. comments
// 2. CDATA
// 3. Processing instructions
// 4. DTD
// 5. tags
// 6. entity references

class XmlDoc;
class QTextDocument;

enum BlockState {
    NO_STATE = 0x0,
    COMMENT_STATE = 0x1,
    CDATA_STATE = 0x2,
    PROCESS_STATE = 0x4,
    DTD_STATE = 0x8,
    INTERNAL_DTD = 0x10
};

class XmlDisplayHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    XmlDisplayHighlighter( QTextDocument *parent = nullptr );

    auto generateTagSet( const XmlDoc &xml ) -> void;

    // not used perhaps for future color themes
    auto commentFormat( const QTextCharFormat &format ) -> void;
    auto cdataFormat( const QTextCharFormat &format ) -> void;
    auto processFormat( const QTextCharFormat &format ) -> void;
    auto dtdFormat( const QTextCharFormat &format ) -> void;
    auto tagFormat( const QTextCharFormat &format ) -> void;
    auto enetityFormat( const QTextCharFormat &format ) -> void;

    // default initialized in ctor, "&lt;" etc...
    auto entityReference( const QString &entityReference ) const -> bool;
    auto addEntityReference( const QString &entityReference ) -> void;
    auto removeEntityReference( const QString &entityReference ) -> bool;

protected:
    virtual auto highlightBlock( const QString &text ) -> void override;

private:
    auto hlBlock( const QString &text, const QRegExp &startExpr, QRegExp endExpr,
                  const QTextCharFormat &format, enum BlockState blockState ) -> void;
    auto hlSet( const QString &text, const QRegExp &pattern,
                const QTextCharFormat &format ) -> void;

    QVector<QRegExp> tags_;
    QSet<QString> entityReferences_;

    QTextCharFormat commentFormat_;
    QTextCharFormat cdataFormat_;
    QTextCharFormat processFormat_;
    QTextCharFormat dtdFormat_;
    QTextCharFormat tagFormat_;
    QTextCharFormat entityFormat_;
};
