#pragma once
#include <QPlainTextEdit>
#include "text_display_highlighter.hpp"

class TextDisplay : public QPlainTextEdit {
    Q_OBJECT
public:
    TextDisplay( QWidget *parent = nullptr );
    TextDisplay( const QString &text, QWidget *parent = nullptr );
    virtual ~TextDisplay( );

    auto addOntology( const QColor &color ) -> void;
    auto removeOntology( int ontologyIndex ) -> void;
    auto addHighlightRule( int ontologyIndex, const QString &rule ) -> void;
    auto removeHighlightRule( int ontologyIndex, const QString &rule ) -> void;

signals:
    void entrySelected( const QString &entry );

protected:
    auto mousePressEvent( QMouseEvent *event ) -> void override;
    auto mouseReleaseEvent( QMouseEvent *event ) -> void override;

private:
    auto cursorSelection( ) -> void;

    struct cursorSelection {
        QTextCursor begin_, end_;
    } cursorSelection_;
    TextDisplayHighlighter *highlighter_;
    QVector<QColor> ontologyVec_;
};
