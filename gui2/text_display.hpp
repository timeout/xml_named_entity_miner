#pragma once
#include <QPlainTextEdit>
// #include <QString>
#include "text_display_highlighter.hpp"
#include "xml_element.hpp"
#include <map>
#include <string>

class QString;

class TextDisplay : public QPlainTextEdit {
    Q_OBJECT
public:
    TextDisplay( const XmlElement &element = XmlElement{}, QWidget *parent = nullptr );
    virtual ~TextDisplay( );

    auto text() const -> const QString &;
    auto scanKeyword( const QString &ontologyName, const QString &entity,
                      const QColor &color ) -> void;
    auto removeHighlightRule( const QString &entity ) -> void;
    auto removeAllHighlightRules( const QColor &color ) -> void;
    auto unlock( ) -> void;
    auto lock( ) -> void;
    auto isLocked( ) const -> bool;
    auto xmlElement( ) const -> XmlElement;
// auto removeOntology

signals:
    void entrySelected( const QString &entry );

protected:
    auto mousePressEvent( QMouseEvent *event ) -> void override;
    auto mouseReleaseEvent( QMouseEvent *event ) -> void override;

private:
    auto addHighlightRule( const QString &entity, const QColor &color ) -> void;
    auto connections( ) -> void;
    auto cursorSelection( ) -> void;

    struct cursorSelection {
        QTextCursor begin_, end_;
    } cursorSelection_;
    XmlElement xmlElement_;
    TextDisplayHighlighter *highlighter_;
    bool lock_;
    QString text_;
    std::map<std::string, std::string> keywords_;
};

