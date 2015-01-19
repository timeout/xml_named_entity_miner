#pragma once
#include <QPlainTextEdit>

class XmlElement;

class TxtSelectionDisplay : public QPlainTextEdit {
    Q_OBJECT
public:
    TxtSelectionDisplay( QWidget *parent = nullptr );

public slots:
    void setXmlTxt( const XmlElement &element );

private:
};
