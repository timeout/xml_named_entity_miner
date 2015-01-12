#include "mainwindow.hpp"

Mainwindow::Mainwindow( QWidget *parent )
    : QMainWindow{parent}, xml_{}, xmlDisplay_{new XmlDisplay( this )},
      xmlNavigator_{new XmlTree( this )} {

    createActions( );
    createMenus( );

    QDockWidget *dockWidget = new QDockWidget( tr( "Dock Widget" ), this );
    dockWidget->setAllowedAreas( Qt::LeftDockWidgetArea );
    dockWidget->setWidget( xmlNavigator_ );
    dockWidget->setFeatures( QDockWidget::DockWidgetVerticalTitleBar );
    addDockWidget( Qt::LeftDockWidgetArea, dockWidget );

    setCentralWidget( xmlDisplay_ );

    statusBar( )->showMessage( tr( "Ready" ) );

    // context menu for tree view
    connect( xmlNavigator_, SIGNAL( customContextMenuRequested( const QPoint & )), this,
             SLOT( onCustomContextRequest( const QPoint & )) );
}

// public slots
void Mainwindow::open( ) {
    const QString filename = QFileDialog::getOpenFileName(
        this, tr( "Open XML file" ), "/usr/local/home/joe", tr( "Xml Files (*.xml)" ) );
    if ( !filename.isEmpty( ) ) {
        std::ifstream in{filename.toUtf8( ).constData( )};
        xml_ = XmlDoc{in};
        xmlNavigator_->xml( xml_ );
    }
}
void Mainwindow::onCustomContextRequest( const QPoint &pos ) {
    QModelIndex idx = xmlNavigator_->indexAt( pos );
    if ( idx.isValid( ) ) {
        xmlTreeContextMenu_->exec( xmlNavigator_->mapToGlobal( pos ) );
    }
}

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
    openAct_ = new QAction{tr( "&Open" ), this};
    openAct_->setShortcuts( QKeySequence::Open );
    openAct_->setStatusTip( tr( "Open an existing file" ) );
    connect( openAct_, SIGNAL( triggered( ) ), this, SLOT( open( ) ) );
}

