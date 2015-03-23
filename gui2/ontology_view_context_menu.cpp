#include "ontology_view_context_menu.hpp"
#include <QAction>

OntologyViewContextMenu::OntologyViewContextMenu( QWidget *parent )
    : QMenu{parent}
    , removeAct_{new QAction{tr( "Remove entity" ), this}}
    , sortAscAct_{new QAction{tr( "Sort ascending" ), this}}
    , sortDescAct_{new QAction{tr( "Sort descending" ), this}}
    , createSynonymAct_{new QAction{tr( "New synonym" ), this}} {
    configActions( );
    connections( );
}

auto OntologyViewContextMenu::configActions( ) -> void {
    removeAct_->setShortcuts( QKeySequence::Delete );
    removeAct_->setStatusTip( tr( "Remove entity" ) );
    removeAct_->setIcon(
        QIcon::fromTheme( "trash-empty", QIcon{":/trash-empty"} ) );
    sortAscAct_->setStatusTip( tr( "Sort ascending" ) );
    sortAscAct_->setIcon(
        QIcon::fromTheme( "view-sort-ascending", QIcon{":/view-sort-ascending"} ) );
    sortDescAct_->setStatusTip( tr( "Sort " ) );
    sortDescAct_->setIcon( QIcon::fromTheme(
        "view-sort-descending", QIcon{":/view-sort-descending"} ) );
    createSynonymAct_->setShortcuts( QKeySequence::New );
    createSynonymAct_->setStatusTip( tr( "Add new synonym" ) );
    createSynonymAct_->setIcon(
        QIcon::fromTheme( "draw-text", QIcon{":/draw-text"} ) );
    addAction( removeAct_ );
    addAction( sortAscAct_ );
    addAction( sortDescAct_ );
    addAction( createSynonymAct_ );
}

auto OntologyViewContextMenu::connections( ) -> void {
    connect( removeAct_, &QAction::triggered, this,
             &OntologyViewContextMenu::removeEntity );
    connect( sortAscAct_, &QAction::triggered, this,
             &OntologyViewContextMenu::sortAscending );
    connect( sortDescAct_, &QAction::triggered, this,
             &OntologyViewContextMenu::sortDescending );
    connect( createSynonymAct_, &QAction::triggered, this,
             &OntologyViewContextMenu::createSynonym );
}
