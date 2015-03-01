#pragma once
#include "xml_element.hpp"
#include <list>
#include <map>
#include <QColor>
#include <QStackedWidget>
#include <QString>

class QWidget;

class StackedTextDisplay : public QStackedWidget {
    Q_OBJECT
public:
    StackedTextDisplay( QWidget *parent = nullptr );

public slots:
    void clear( );
    void addElement( const XmlElement &element );
    void removeElement( const XmlElement &element );
    void addOntology( const QString &ontologyName, const QColor formatColor );
    void removeOntology( const QString &ontologyName );
    void highlight( const QString &ontologyName, const QString &entiy );
    void next( );
    void prev( );

signals:
    void entitySelected( const QString &entity );
    void end( );
    void begin( );

private:
    auto beginEnd( ) -> void;
    std::list<XmlElement> elements_;
    std::list<XmlElement>::iterator pos_;
    std::map<QString, QColor> ontologyMap_;
};
