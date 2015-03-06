#pragma once
#include <QPlainTextEdit>
#include <QString>
#include "text_display_highlighter.hpp"

class TextDisplay : public QPlainTextEdit {
    Q_OBJECT
public:
    TextDisplay( const QString &text = QString{}, QWidget *parent = nullptr );
    virtual ~TextDisplay( );

    auto addHighlightRule( const QString &entity, const QColor &color ) -> void;
    auto removeHighlightRule( const QString &entity ) -> void;
    auto unlock( ) -> void;
    auto lock( ) -> void;
    auto isLocked( ) const -> bool;

signals:
    void entrySelected( const QString &entry );

protected:
    auto mousePressEvent( QMouseEvent *event ) -> void override;
    auto mouseReleaseEvent( QMouseEvent *event ) -> void override;

private:
    auto connections( ) -> void;
    auto cursorSelection( ) -> void;

    struct cursorSelection {
        QTextCursor begin_, end_;
    } cursorSelection_;
    TextDisplayHighlighter *highlighter_;
    QVector<QColor> ontologyVec_;
    bool lock_;
};
