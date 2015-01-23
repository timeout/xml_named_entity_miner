#include "mainwindow.hpp"
#include "xml_display.hpp"
#include "txt_selection_display.hpp"
#include "xml_tree.hpp"
#include "entity_tree.hpp"
#include "xml_doc.hpp"
#include "element_selections.hpp"
#include "synonyms.hpp"

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
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QStringList>

#include <QString>
#include <QDebug>

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
    fileToolBar_->addAction( exitAct_ );
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
    connect( txtSelectionDisplay_, SIGNAL( entrySelected( const QString & )),
             entityNavigator_, SLOT( insertEntry( const QString & )) );
    connect( txtSelectionDisplay_, SIGNAL( textChanged( ) ), entityNavigator_,
             SLOT( dictionary( ) ) );
    connect( entityNavigator_, SIGNAL( dictionaryRequested( const Dictionary & )),
             txtSelectionDisplay_, SLOT( scan( const Dictionary & )) );
}

// public slots
void Mainwindow::open( ) {
    const QString filename = QFileDialog::getOpenFileName(
        this, tr( "Open XML file" ), "/usr/local/home/joe", tr( "Xml Files (*.xml)" ) );
    if ( !filename.isEmpty( ) ) {
        loadFile( filename );
        //
        // std::ifstream in{filename.toUtf8( ).constData( )};
        // XmlDoc xml{in};
        // xmlNavigator_->xml( xml );
        // xmlDisplay_->setPlainText( QString::fromUtf8( xml.toString( ).c_str( ) ) );
        // xmlDisplay_->setXml( xml );
    }
}

void Mainwindow::onCustomContextRequest( const QPoint &pos ) {
    QModelIndex idx = xmlNavigator_->indexAt( pos );
    if ( idx.isValid( ) ) {
        xmlTreeContextMenu_->exec( xmlNavigator_->mapToGlobal( pos ) );
    }
}

void Mainwindow::loadFile( const QString &fileName ) {
    QFile file( fileName );
    std::ifstream in{fileName.toStdString( )};
    if ( !in.is_open( ) ) {
        QMessageBox::warning( this, tr( "Recent Files" ),
                              tr( "Cannot read file %1:\n%2." ).arg( fileName ).arg(
                                  file.errorString( ) ) );
        return;
    }
    // TODO: refactor clear selections
    selections_->clear( );

    XmlDoc xml{in}; // TODO: error checking
    QApplication::setOverrideCursor( Qt::WaitCursor );
    // TODO:
    // xmlNavigator_->xml( xml );
    xmlDisplay_->setPlainText( QString::fromUtf8( xml.toString( ).c_str( ) ) );
    xmlDisplay_->setXml( xml );
    QApplication::restoreOverrideCursor( );

    setCurrentFile( fileName );
    statusBar( )->showMessage( tr( "File loaded" ), 2000 );
}

void Mainwindow::setCurrentFile( const QString &fileName ) {
    curFile_ = fileName;
    setWindowFilePath( curFile_ );

    QSettings settings;
    QStringList files = settings.value( "recentFileList" ).toStringList( );
    files.removeAll( fileName );
    files.prepend( fileName );
    // foreach( const QString& file, files ) { qDebug( ) << "file: " << file << "\n"; }
    while ( files.size( ) > MaxRecentFiles ) {
        files.removeLast( );
    }

    settings.setValue( "recentFileList", files );

    foreach ( QWidget *widget, QApplication::topLevelWidgets( ) ) {
        Mainwindow *mainWin = qobject_cast<Mainwindow *>( widget );
        if ( mainWin )
            mainWin->updateRecentFileActions( );
    }
}

void Mainwindow::onNextSelectionTriggered( ) {}
void Mainwindow::onPreviousSelectionTriggered( ) {}
void Mainwindow::openRecentFile( ) {
    QAction *action = qobject_cast<QAction *>( sender( ) );
    if ( action ) {
        loadFile( action->data( ).toString( ) );
    }
}

// private member functions
void Mainwindow::createMenus( ) {

    QMenu *fileMenu = menuBar( )->addMenu( tr( "&File" ) );
    fileMenu->addAction( openAct_ );
    recentFilesMenu_ = fileMenu->addMenu( QIcon::fromTheme( "application-menu" ),
                                          tr( "Recent &Files" ) );
    for ( int i = 0; i < MaxRecentFiles; ++i )
        recentFilesMenu_->addAction( recentFileActs[i] );
    fileMenu->addAction( exitAct_ );
    updateRecentFileActions( );

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
    exitAct_ = new QAction( tr( "E&xit" ), this );
    exitAct_->setShortcuts( QKeySequence::Quit );
    exitAct_->setStatusTip( tr( "Exit the application" ) );
    exitAct_->setIcon( QIcon::fromTheme( "application-exit" ) );
    connect( exitAct_, SIGNAL( triggered( ) ), this, SLOT( close( ) ) );

    for ( int i = 0; i < MaxRecentFiles; ++i ) {
        recentFileActs[i] = new QAction( this );
        // recentFileActs[i]->setVisible( false );
        recentFileActs[i]->setVisible( true );
        connect( recentFileActs[i], SIGNAL( triggered( ) ), this,
                 SLOT( openRecentFile( ) ) );
    }

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

void Mainwindow::updateRecentFileActions( ) {
    QSettings settings;
    QStringList files = settings.value( "recentFileList" ).toStringList( );

    int numRecentFiles = qMin( files.size( ), ( int )MaxRecentFiles );

    for ( int i = 0; i < numRecentFiles; ++i ) {
        QString text = tr( "&%1 %2" ).arg( i + 1 ).arg( strippedName( files[i] ) );
        recentFileActs[i]->setText( text );
        recentFileActs[i]->setData( files[i] );
        recentFileActs[i]->setVisible( true );
    }
    for ( int j = numRecentFiles; j < MaxRecentFiles; ++j )
        recentFileActs[j]->setVisible( false );

    //  separatorAct->setVisible(numRecentFiles > 0);
}

QString Mainwindow::strippedName( const QString &fullFileName ) {
    return QFileInfo( fullFileName ).fileName( );
}

