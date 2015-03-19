#include "tabbed_ontology_view.hpp"
#include "ontology_view.hpp"
#include "add_ontology_dialog.hpp"
#include "xml_element.hpp"
#include <QColor>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QTabBar>
#include <QToolButton>
#include <QWidget>
#include <cassert>

#include <iostream>
#include <QDebug>

TabbedOntologyView::TabbedOntologyView( QWidget *parent )
    : QTabWidget{parent}, newTabButton_{new QToolButton} {
    newTabButton_->setIcon( QIcon::fromTheme( "add", QIcon{":images/add.svg"} ) );
    newTabButton_->setAutoRaise( true );
    newTabButton_->setToolTip( tr( "Add a new Ontology" ) );

    addTab( new QLabel, QString{} );
    setTabEnabled( 0, false );
    tabBar( )->setTabButton( 0, QTabBar::RightSide, newTabButton_ );
    setTabPosition( QTabWidget::West );

    setTabsClosable( true );

    connections( );
}

auto TabbedOntologyView::thesaurus( const QString &ontologyName ) const -> const Thesaurus
    & {
    for ( int i = 0; i < count( ) - 1; ++i ) {
        OntologyView *ontologyView = reinterpret_cast<OntologyView *>( widget( i ) );
        if ( ontologyName == ontologyView->ontologyName( ) ) {
            return ontologyView->thesaurus( );
        }
    }
    assert( false );
}

void TabbedOntologyView::clearOntologyViews( ) {
    for ( int i = 0; i < count( ) - 1; ++i ) {
        OntologyView *ontologyView = reinterpret_cast<OntologyView *>( widget( i ) );
        ontologyView->clear( );
    }
}

void TabbedOntologyView::addEntity( const QString &entity ) {
    qDebug( ) << "TabbedOntologyView#addEntity( " << entity << " )";
    // add entity to current ontology view
    if ( count( ) > 1 ) {
        OntologyView *ontologyView = reinterpret_cast<OntologyView *>( currentWidget( ) );
        ontologyView->addEntity( entity );
    } else {
        QMessageBox::warning( this, tr( "WARNING: Ontology not configured" ),
                              tr( "No ontologies exist!\nEntities cannot be selected "
                                  "before you have configured an ontology." ) );
        return;
    }
}

void TabbedOntologyView::scanText( const QString &text ) {
    std::cerr << __PRETTY_FUNCTION__ << " scanning text" << std::endl;
    if ( count( ) == 1 ) {
        return;
    }
    for ( int i = 0; i < count( ) - 1; ++i ) {
        OntologyView *ontologyView = reinterpret_cast<OntologyView *>( widget( i ) );
        ontologyView->scanText( text );
    }
}

void TabbedOntologyView::addOntology( ) {
    AddOntologyDialog dialog{this};
    if ( dialog.exec( ) ) {
        QString ontologyName = dialog.name( );
        QColor color = dialog.color( );

        // TODO: debug
        qDebug( ) << "new ontology name: " << ontologyName << "\n";
        qDebug( ) << "new ontology color: " << color << "\n";

        // create ontology view
        qDebug( ) << "creating new ontology view...";
        addOntologyTab( ontologyName, color );

        // create new ontology
        // qDebug( ) << "creating new ontology...";
        // ontologies_->addOntology( );
        emit ontologyAdded( ontologyName, color );
    }
}

void TabbedOntologyView::closeTab( int index ) {
    OntologyView *ontologyView = reinterpret_cast<OntologyView *>( widget( index ) );
    if ( ontologyView ) {
        auto ontologyName = ontologyView->ontologyName( );
        delete ontologyView;
        emit ontologyRemoved( ontologyName );
    }
}

void TabbedOntologyView::addOntologyTab( const QString &ontologyName,
                                         const QColor &color ) {
    OntologyView *ontologyView = new OntologyView{ontologyName, color};
    ontologyView->setContextMenuPolicy( Qt::CustomContextMenu );
    auto index = insertTab( count( ) - 1, ontologyView, ontologyName );
    tabBar( )->setTabTextColor( index, color );
    setCurrentIndex( index );
    connect( ontologyView, &OntologyView::entityAdded, this,
             &TabbedOntologyView::entityAdded );
    connect( ontologyView, &OntologyView::entityRemoved, this,
             &TabbedOntologyView::entityRemoved );
}

void TabbedOntologyView::removeOntologyTab( int index ) {
    OntologyView *ontologyView = reinterpret_cast<OntologyView *>( widget( index ) );
    removeTab( index );
    delete ontologyView;
}

auto TabbedOntologyView::connections( ) -> void {
    connect( newTabButton_, &QToolButton::clicked, this,
             &TabbedOntologyView::addOntology );
    connect( this, &TabbedOntologyView::tabCloseRequested, this,
             &TabbedOntologyView::closeTab );
}

