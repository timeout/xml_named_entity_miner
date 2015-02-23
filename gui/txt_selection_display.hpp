#pragma once
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>

class XmlElement;
class Dictionary;
class QTextDocument;

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    Highlighter( QTextDocument *parent = nullptr );

protected:
    void highlightBlock( const QString &text ) override;
};


class TxtSelectionDisplay : public QPlainTextEdit {
    Q_OBJECT
public:
    TxtSelectionDisplay( QWidget *parent = nullptr );

protected:
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

public slots:
    void setXmlTxt( const XmlElement &element );
// void scan( const Dictionary &dictionary );

signals:
    void entrySelected( const QString &entry );

private:
    void cursorSelection( );
    const QList<QTextEdit::ExtraSelection> findAll( const QString &textSelection );

    struct cursorSelection {
        QTextCursor begin_, end_;
    } cursorSelection_;
    QList<QTextEdit::ExtraSelection> extraSelections_;

    Highlighter *highlighter_;
};
