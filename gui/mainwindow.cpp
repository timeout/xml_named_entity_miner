#include "mainwindow.hpp"
#include "xml_display.hpp"
#include "txt_selection_display.hpp"
#include "xml_tree.hpp"
#include "entity_tree.hpp"
#include "xml_doc.hpp"
#include "element_selections.hpp"

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QPoint>
#include <QDockWidget>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QIcon>

#include <string>
#include <fstream>

Mainwindow::Mainwindow( QWidget *parent )
    : QMainWindow{parent}, xmlDisplay_{new XmlDisplay( this )},
      txtSelectionDisplay_{new TxtSelectionDisplay( this )},
      selections_{new ElementSelections( this )}, xmlNavigator_{new XmlTree( this )},
      entityNavigator_{new EntityTree( this )} {

    // init
    createActions( );

    // menus
    createMenus( );

    // toolbar
    fileToolBar_ = addToolBar( "ApplicationToolBar" );
    fileToolBar_->setMovable( false );
    fileToolBar_->setAllowedAreas( Qt::TopToolBarArea );
    fileToolBar_->addAction( openAct_ );
    fileToolBar_->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    documentToolBar_ = addToolBar( "DocumentToolBar" );
    documentToolBar_->setMovable( false );
    documentToolBar_->setAllowedAreas( Qt::TopToolBarArea );
    documentToolBar_->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    documentToolBar_->addAction( nextSelectionAct_ );
    documentToolBar_->addAction( previousSelectionAct_ );

    // docks
    // left dock
    QDockWidget *xmlNavDock = new QDockWidget( tr( "Xml Navigator" ), this );
    xmlNavDock->setAllowedAreas( Qt::LeftDockWidgetArea );
    xmlNavDock->setWidget( xmlNavigator_ );
    xmlNavDock->setFeatures( QDockWidget::DockWidgetVerticalTitleBar );
    addDockWidget( Qt::LeftDockWidgetArea, xmlNavDock );

    QDockWidget *entityNavDock = new QDockWidget( tr( "Entity Navigator" ), this );
    entityNavDock->setAllowedAreas( Qt::RightDockWidgetArea );
    entityNavDock->setWidget( entityNavigator_ );
    entityNavDock->setFeatures( QDockWidget::DockWidgetVerticalTitleBar );
    addDockWidget( Qt::RightDockWidgetArea, entityNavDock );

    // ==debug
    entityNavigator_->insertEntry( QString{"first entry"} );
    entityNavigator_->insertEntry( QString{"second entry"} );
    entityNavigator_->insertEntry( QString{"first entry"} );
    entityNavigator_->insertEntry( QString{"third entry"} );
    entityNavigator_->insertEntry( QString{"fourth entry"} );

    // central widget
    tabWidget_ = new QTabWidget;
    tabWidget_->addTab( xmlDisplay_, tr( "&Overview" ) );
    tabWidget_->addTab( txtSelectionDisplay_, tr( "&Selection" ) );
    tabWidget_->setTabPosition( QTabWidget::South );
    setCentralWidget( tabWidget_ );

    // status bar
    statusBar( )->showMessage( tr( "Ready" ) );

    // context menu for tree view
    connect( xmlNavigator_, SIGNAL( customContextMenuRequested( const QPoint & )), this,
             SLOT( onCustomContextRequest( const QPoint & )) );

    connect( xmlNavigator_, SIGNAL( xmlItemSelected( const XmlElement & )), selections_,
             SLOT( addXmlElement( const XmlElement & )) );
    connect( xmlNavigator_, SIGNAL( xmlItemDeselected( const XmlElement & )), selections_,
             SLOT( removeXmlElement( const XmlElement & )) );
    connect( selections_, SIGNAL( currentXmlElement( const XmlElement & )),
             txtSelectionDisplay_, SLOT( setXmlTxt( const XmlElement & )) );
    connect( selections_, SIGNAL( currentXmlElementInvalid( ) ), txtSelectionDisplay_,
             SLOT( clear( ) ) );
}

// public slots
void Mainwindow::open( ) {
    const QString filename = QFileDialog::getOpenFileName(
        this, tr( "Open XML file" ), "/usr/local/home/joe", tr( "Xml Files (*.xml)" ) );
    if ( !filename.isEmpty( ) ) {
        std::ifstream in{filename.toUtf8( ).constData( )};
        XmlDoc xml{in};
        xmlNavigator_->xml( xml );
        // xmlDisplay_->setPlainText( QString::fromUtf8( xml.toString( ).c_str( ) ) );
        xmlDisplay_->setXml( xml );
    }
}

void Mainwindow::onCustomContextRequest( const QPoint &pos ) {
    QModelIndex idx = xmlNavigator_->indexAt( pos );
    if ( idx.isValid( ) ) {
        xmlTreeContextMenu_->exec( xmlNavigator_->mapToGlobal( pos ) );
    }
}

void Mainwindow::onNextSelectionTriggered( ) {}
void Mainwindow::onPreviousSelectionTriggered( ) {}

// private member functions
void Mainwindow::createMenus( ) {
    QMenu *fileMenu = menuBar( )->addMenu( tr( "&File" ) );
    fileMenu->addAction( openAct_ );

    xmlTreeContextMenu_ = new QMenu( this );
    // TODO: create actions: select all, clear selections?
    // stub
    xmlTreeContextMenu_->addAction( openAct_ );
}

void Mainwindow::createActions( ) {
    // file actions
    openAct_ = new QAction{tr( "&Open" ), this};
    openAct_->setShortcuts( QKeySequence::Open );
    openAct_->setStatusTip( tr( "Open an existing file" ) );
    openAct_->setIcon( QIcon::fromTheme( "document-open" ) );
    connect( openAct_, SIGNAL( triggered( ) ), this, SLOT( open( ) ) );

    // document actions
    nextSelectionAct_ = new QAction{tr( "Next Selection" ), this};
    nextSelectionAct_->setStatusTip( tr( "Activate next selection" ) );
    nextSelectionAct_->setIcon( QIcon::fromTheme( "arrow-right" ) );
    connect( nextSelectionAct_, SIGNAL( triggered( ) ), selections_, SLOT( next( ) ) );

    previousSelectionAct_ = new QAction{tr( "Previous Selection" ), this};
    previousSelectionAct_->setStatusTip( tr( "Activate previous selection" ) );
    previousSelectionAct_->setIcon( QIcon::fromTheme( "arrow-left" ) );
    connect( previousSelectionAct_, SIGNAL( triggered( ) ), selections_,
             SLOT( previous( ) ) );
}

