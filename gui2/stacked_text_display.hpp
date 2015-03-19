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
    auto ontologyMap( ) const -> const std::map<QString, QColor> & {
        return ontologyMap_;
    }

public slots:
    void clear( );
    void addElement( const XmlElement &element );
    void removeElement( const XmlElement &element );
    void addOntology( const QString &ontologyName, const QColor formatColor );
    void removeOntology( const QString &ontologyName );
    void highlight( const QString &ontologyName, const QString &entity );
    void removeHighlight( const QString &entity );
    void next( );
    void prev( );
    void unlock( );
    void lock( );

signals:
    void entitySelected( const QString &entity );
    void enableNext( bool enable );
    void enablePrev( bool enable );
    void textAdded( const QString &text );
    void hasContent( bool enable );
    void disabled( bool isDisabled );

private:
    auto currentTextDisplayLocked( ) const -> bool;
    auto beginEnd( ) -> void;
    auto connections( ) -> void;
    std::list<XmlElement> elements_;
    std::list<XmlElement>::iterator pos_;
    std::map<QString, QColor> ontologyMap_;
};
