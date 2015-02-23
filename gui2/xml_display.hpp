#pragma once
#include <QPlainTextEdit>

class XmlDoc;
class XmlDisplayHighlighter;

class XmlDisplay : public QPlainTextEdit {
    Q_OBJECT
public:
    XmlDisplay( QWidget *parent = nullptr );
    auto setXml( const XmlDoc &xml ) -> void;

private:
    XmlDisplayHighlighter *xmlDisplayHighlighter_;
};
