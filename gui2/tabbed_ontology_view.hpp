#pragma once
#include <QTabWidget>

class XmlElement;
class QColor;
class QString;
class QToolButton;
class QWidget;
class Thesaurus;

class TabbedOntologyView : public QTabWidget {
    Q_OBJECT
public:
    explicit TabbedOntologyView( QWidget *parent = nullptr );
    auto thesaurus( const QString &ontologyName ) const -> const Thesaurus &;

public slots:
    void clearOntologyViews( );
    void addEntity( const QString &entity );
    void scanText( const QString &text );
    void addOntology( );
    void closeTab( int index );

signals:
    void entityRemoved( const QString &entity );
    void ontologyAdded( const QString &ontologyName, const QColor color );
    void ontologyRemoved( const QString &ontologyName );
    void entityAdded( const QString &ontologyName, const QString &entity );

private slots:
    void removeOntologyTab( int index );
    void addOntologyTab( const QString &ontologyName, const QColor &color );

private:
    auto connections( ) -> void;

    QToolButton *newTabButton_;
};
