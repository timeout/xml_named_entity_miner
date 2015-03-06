#pragma once
#include <QSyntaxHighlighter>

class TextDisplayHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    TextDisplayHighlighter( QTextDocument *document = nullptr );
    auto addRule( const QString &rule, const QColor &color ) -> void;
    auto removeRule( const QString &rule ) -> void;
    auto removeAll( const QColor &color ) -> void;

public slots:
    void rehighlightModification( bool isModified );

protected:
    virtual auto highlightBlock( const QString &text ) -> void override;

private:
    QMap<QString, QTextCharFormat> rules_;
};
