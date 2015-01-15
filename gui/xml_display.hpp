#pragma once

#include <QPlainTextEdit>

class XmlDoc;
class XmlHighlighter;

class XmlDisplay : public QPlainTextEdit {
    Q_OBJECT
public:
    XmlDisplay( QWidget *parent = nullptr );
    void setXml( const XmlDoc &xml );

private:
    XmlHighlighter *xmlHighlighter;
};
