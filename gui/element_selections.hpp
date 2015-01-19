#pragma once
#include "xml_element.hpp"
#include <QObject>
#include <list>

class XmlDoc;

class ElementSelections : public QObject {
    Q_OBJECT
public:
    ElementSelections( QObject *parent = nullptr );

public slots:
    void clear( );
    void addXmlElement( const XmlElement &element );
    void removeXmlElement( const XmlElement &element );
    void next( );
    void previous( );

signals:
    void currentXmlElementInvalid( );
    void currentXmlElement( const XmlElement &element );
    void currentIdx(int idx);

private:
    std::size_t num_;
    std::list<XmlElement> selections_;
    std::list<XmlElement>::iterator idx_;
};
