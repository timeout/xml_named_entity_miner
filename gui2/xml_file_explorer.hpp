#pragma once
#include <QWidget>
#include "xml_doc.hpp"

class XmlElement;
class XmlFileOutline;
class XPathQueryWidget;

class XmlFileExplorer : public QWidget {
    Q_OBJECT
public:
    explicit XmlFileExplorer( QWidget *parent = nullptr );

public slots:
    void clear( );
    void setXml( const XmlDoc &xml );

signals:
    void elementSelected( const XmlElement &element );
    void elementDeselected( const XmlElement &element );

private:
    auto layout() -> void;
    auto connections() -> void;

    XmlFileOutline *xmlFileOutline_;
    XPathQueryWidget *xPathQueryWidget_;
    XmlDoc xml_;
};
