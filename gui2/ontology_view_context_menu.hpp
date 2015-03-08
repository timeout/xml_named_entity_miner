#pragma once
#include <QMenu>

class QAction;

class OntologyViewContextMenu : public QMenu {
    Q_OBJECT
public:
    OntologyViewContextMenu( QWidget *parent = nullptr );

signals:
    void removeEntity( );
    void sortAscending( );
    void sortDescending( );
    void createSynonym( );

private:
    auto configActions( ) -> void;
    auto connections( ) -> void;
    QAction *removeAct_, *sortAscAct_, *sortDescAct_, *createSynonymAct_;
};
