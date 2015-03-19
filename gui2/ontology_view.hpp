#pragma once
#include <QTreeWidget>
#include "dictionary.hpp"
#include "thesaurus.hpp"

class Dictionary;
class QAction;
class QMenu;
class QPoint;
class QTreeWidgetItem;

class OntologyView : public QTreeWidget {
    Q_OBJECT

public:
    OntologyView( const QString &ontologyName = QString{}, const QColor &color = QColor{},
                  QWidget *parent = nullptr );
    auto clear( ) -> void;
    auto ontologyName( const QString &name ) -> void;
    auto ontologyName( ) const -> const QString &;
    auto color( const QColor &color ) -> void;
    auto color( ) const -> const QColor;
    auto addEntity( const QString &entity ) -> int;
    auto removeEntity( const QString &entity ) -> void;
    auto addSynonym( const QString &entity, const QString &canon ) -> void;
    auto removeSynonym( const QString &entity ) -> void;
    auto scanText( const QString &contents ) -> void;
    auto thesaurus( ) const -> const Thesaurus & { return thesaurus_; }

public slots:
    void removeSelectedItem( );
    void sortAscending( );
    void sortDescending( );
    void createSynonym( );
    void contextMenuRequest( const QPoint &pos );

signals:
    void entityAdded( const QString ontologyName, const QString &entity );
    void entityRemoved( const QString &entity );

protected:
    auto dropEvent( QDropEvent *event ) -> void override;

private:
    auto insertItem( const QString &entity ) -> void;
    auto contains( const QString &entity ) -> bool;
    auto connections( ) -> void;

    QTreeWidgetItem *selectedItem_;

    Dictionary dictionary_;
    Thesaurus thesaurus_;
    QString ontologyName_;
    QColor color_;
};

