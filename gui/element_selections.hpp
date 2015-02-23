#pragma once
#include "xml_element.hpp"
#include <QObject>
#include <QString>
#include <QVector>
#include <QMultiMap>
#include <QPair>
#include <list>
#include <QMetaType>

class XmlDoc;

class Selection {
public:
    Selection( ) {}
    Selection( const XmlElement &element, const QString &text,
               const QVector<QVector<QString>> &keywords )
        : element_{element}, text_{text}, keywords_{keywords} {}
    auto element( ) const -> const XmlElement & { return element_; }
    auto text( ) const -> const QString & { return text_; }
    auto keywords( ) const -> const QVector<QVector<QString>> { return keywords_; }
    auto operator<( const Selection &other ) const -> bool {
        return element_ < other.element_;
    }
    auto operator==( const Selection &other ) const -> bool {
        return element_ == other.element_;
    }

private:
    XmlElement element_;
    QString text_;
    QVector<QVector<QString>> keywords_;
};

Q_DECLARE_METATYPE( Selection )

class ElementSelections : public QObject {
    Q_OBJECT
public:
    ElementSelections( QObject *parent = nullptr );

public slots:
    void clear( );
    void addSelection( const Selection &selection );
    void removeSelection( const XmlElement &element );
    // void addXmlElement( const XmlElement &element );
    // void removeXmlElement( const XmlElement &element );
    void next( );
    void previous( );

signals:
    void selectionAdded( int index, const QString &text );
    void selectionRemoved( int index );
    void currentIndexChanged( int index );
    void clearSelections( );
    void keywordAdded( int index, int ontologyIndex, const QString &keyword );
    // void currentXmlElementInvalid( );
    // void currentXmlElement( const XmlElement &element );
    // void currentIdx( int idx );

private:
    int index( ) { return ( pos_ - std::begin( selections_ ) ); }
    // int index_;
    // std::list<XmlElement> selections_;
    // std::list<XmlElement>::iterator idx_;
    QVector<Selection> selections_;
    QVector<Selection>::const_iterator pos_;
};
