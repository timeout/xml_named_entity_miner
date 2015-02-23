#include "mainwindow.hpp"
#include "xml_display.hpp"
#include "stacked_text_display.hpp"
#include "xml_file_outline.hpp"
#include "ontology_view.hpp"
#include "xml_doc.hpp"
#include "element_selections.hpp"
#include "synonyms.hpp"
#include "ontology_dialog.hpp"
#include "xpath_query_widget.hpp"
#include "xml_task_dialog.hpp"

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
#include <QToolButton>
#include <QLabel>
#include <QGridLayout>
#include <QErrorMessage>

#include <QString>
#include <QDebug>

#include <string>
#include <fstream>
#include <sstream>

Mainwindow::Mainwindow( QWidget *parent )
    : QMainWindow{parent}, xmlDisplay_{new XmlDisplay( this )},
      stackedTextDisplay_{new StackedTextDisplay{this}},
      xmlFileOutline_{new XmlFileOutline{this}}, xPathQueryWidget_{new XPathQueryWidget},
      ontologies_{new Ontologies{this}}, selections_{new ElementSelections{this}} {

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
    QWidget *leftDockWidget = new QWidget;
    QGridLayout *leftDockLayout = new QGridLayout;
    leftDockLayout->addWidget( xmlFileOutline_, 0, 0 );
    leftDockLayout->addWidget( xPathQueryWidget_, 1, 0 );
    leftDockLayout->setRowStretch( 0, 3 );
    leftDockLayout->setRowStretch( 1, 1 );
    leftDockLayout->setContentsMargins( 0, 0, 0, 0 );

    leftDockWidget->setLayout( leftDockLayout );

    QDockWidget *xmlNavDock = new QDockWidget( tr( "Xml File Outline" ), this );
    xmlNavDock->setAllowedAreas( Qt::LeftDockWidgetArea );
    xmlNavDock->setWidget( leftDockWidget );
    xmlNavDock->setFeatures( QDockWidget::DockWidgetVerticalTitleBar );
    addDockWidget( Qt::LeftDockWidgetArea, xmlNavDock );

    // right dock
    tabOntologies_ = new QTabWidget;
    tabOntologies_->setTabsClosable( true );
    QToolButton *tb = new QToolButton( );
    tb->setText( "+" );
    tb->setAutoRaise( true );
    tb->setToolTip( tr( "Click to add a new Ontology" ) );

    // ontology dialog
    connect( tb, SIGNAL( clicked( ) ), this, SLOT( setNewOntology( ) ) );
    connect( ontologies_, SIGNAL( noOntologySet( ) ), this, SLOT( setNewOntology( ) ) );

    tabOntologies_->addTab( new QLabel( "You can add tabs by pressing <b>\"+\"</b>" ),
                            QString{} );
    tabOntologies_->setTabEnabled( 0, false );
    tabOntologies_->tabBar( )->setTabButton( 0, QTabBar::RightSide, tb );
    tabOntologies_->setTabPosition( QTabWidget::West );

    connect( tabOntologies_, SIGNAL( tabCloseRequested( int )), SLOT( closeTab( int )) );
    connect( tabOntologies_, SIGNAL( currentChanged( int )), ontologies_,
             SLOT( setCurrentIndex( int )) );

    QDockWidget *entityNavDock = new QDockWidget( tr( "Ontology" ), this );
    entityNavDock->setAllowedAreas( Qt::RightDockWidgetArea );
    entityNavDock->setWidget( tabOntologies_ );
    entityNavDock->setFeatures( QDockWidget::NoDockWidgetFeatures );
    addDockWidget( Qt::RightDockWidgetArea, entityNavDock );

    // central widget
    tabWidget_ = new QTabWidget;
    tabWidget_->addTab( xmlDisplay_, tr( "&Overview" ) );
    tabWidget_->addTab( stackedTextDisplay_, tr( "&Selection" ) );
    tabWidget_->setTabPosition( QTabWidget::South );
    setCentralWidget( tabWidget_ );

    // status bar
    statusBar( )->showMessage( tr( "Ready" ) );

    // context menu for tree view
    connect( xmlFileOutline_, SIGNAL( customContextMenuRequested( const QPoint & )), this,
             SLOT( onCustomContextRequest( const QPoint & )) );

    connect( xmlFileOutline_, SIGNAL( xmlItemSelected( const XmlElement & )),
             SLOT( generateInitialSelection( const XmlElement & )) );
    connect( selections_, SIGNAL( selectionAdded( int, const QString & )),
             stackedTextDisplay_, SLOT( insertDisplay( int, const QString & )) );
    connect( selections_, SIGNAL( selectionRemoved( int )), stackedTextDisplay_,
             SLOT( removeDisplay( int )) );
    connect( selections_, SIGNAL( currentIndexChanged( int )), stackedTextDisplay_,
             SLOT( setCurrentIndex( int )) );
    connect( selections_, SIGNAL( clearSelections( ) ), stackedTextDisplay_,
             SLOT( clear( ) ) );
    connect( selections_, SIGNAL( keywordAdded( int, int, const QString & )),
             stackedTextDisplay_,
             SLOT( insertHighlightRule( int, int, const QString & )) );
    // connect( xmlFileOutline_, SIGNAL( xmlItemSelected( const XmlElement & )),
    // selections_,
    //          SLOT( addXmlElement( const XmlElement & )) );
    connect( xmlFileOutline_, SIGNAL( xmlItemDeselected( const XmlElement & )),
             selections_, SLOT( removeSelection( const XmlElement & )) );
    connect( ontologies_, SIGNAL( entryAddedToDictionary( const QString &, int )),
             SLOT( addEntryToCurrentTab( const QString &, int )) );
}

// public slots
void Mainwindow::open( ) {
    const QString filename = QFileDialog::getOpenFileName(
        this, tr( "Open XML file" ), "/usr/local/home/joe", tr( "Xml Files (*.xml)" ) );
    if ( !filename.isEmpty( ) ) {
        loadFile( filename );
    }
}

void Mainwindow::onCustomContextRequest( const QPoint &pos ) {
    QModelIndex idx = xmlFileOutline_->indexAt( pos );
    if ( idx.isValid( ) ) {
        xmlTreeContextMenu_->exec( xmlFileOutline_->mapToGlobal( pos ) );
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

    xml_ = XmlDoc{in}; // TODO: error checking
    if ( xml_ ) {
        QApplication::setOverrideCursor( Qt::WaitCursor );
        // TODO:
        xmlFileOutline_->xml( xml_ );

        xmlDisplay_->setPlainText( QString::fromUtf8( xml_.toString( ).c_str( ) ) );
        xmlDisplay_->setXml( xml_ );
        QApplication::restoreOverrideCursor( );

        setCurrentFile( fileName );
        statusBar( )->showMessage( tr( "File loaded" ), 2000 );
    } else {
        QString msg;
        if ( xml_.errorHandler( ).hasErrors( ) ) {
            std::ostringstream errors;
            errors << xml_.errorHandler( );
            std::cerr << "Error handler: " << xml_.errorHandler( ) << std::endl;
            msg = QString::fromStdString( errors.str( ) );
        }
        QErrorMessage *errorMessage = new QErrorMessage{this};
        errorMessage->setWindowTitle( "Could not open XML" );
        if ( !msg.isEmpty( ) ) {
            errorMessage->showMessage( msg );
            qDebug( ) << msg;
        }
        qDebug( ) << "Could not open xml\n";
    }
}

void Mainwindow::setCurrentFile( const QString &fileName ) {
    curFile_ = fileName;
    setWindowFilePath( curFile_ );

    QSettings settings;
    QStringList files = settings.value( "recentFileList" ).toStringList( );
    files.removeAll( fileName );
    files.prepend( fileName );
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

void Mainwindow::setNewOntology( ) {
    OntologyDialog dialog{this};
    if ( dialog.exec( ) ) {
        QString name = dialog.name( );
        QColor color = dialog.color( );

        // TODO: debug
        qDebug( ) << "new ontology name: " << name << "\n";
        qDebug( ) << "new ontology color: " << color << "\n";

        // create ontology view
        qDebug( ) << "creating new ontology view...";
        auto idx = createNewOntologyViewTab( name, color );

        // create new ontology
        qDebug( ) << "creating new ontology...";
        ontologies_->addOntology( );
    }
}

void Mainwindow::validateDialog( ) {
    if ( xml_ ) {
        qDebug( ) << "starting the validation dialog...\n";
        XmlSchemaDialog dialog{xml_, this};
        dialog.exec( );
    }
}

void Mainwindow::transformDialog( ) {
    if ( xml_ ) {
        XsltDialog dialog{xml_, this};
        dialog.exec( );
    }
}

void Mainwindow::closeTab( int idx ) {
    // remove tab
    deleteOntologyViewTab( idx );

    // remove ontology
    ontologies_->removeOntology( idx );
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

    QMenu *toolMenu = menuBar( )->addMenu( tr( "&Tools" ) );
    toolMenu->addAction( validateAct_ );
    toolMenu->addAction( transformAct_ );

    QMenu *viewMenu = menuBar( )->addMenu( tr( "&View" ) );
    viewMenu->addAction( nextSelectionAct_ );
    viewMenu->addAction( previousSelectionAct_ );

    xmlTreeContextMenu_ = new QMenu{this};
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

    // tools
    validateAct_ = new QAction{tr( "Xml Schema Validate" ), this};
    validateAct_->setStatusTip(
        tr( "Validated the current XML file using an XML Schema" ) );
    validateAct_->setIcon( QIcon::fromTheme( "dialog-yes" ) );
    connect( validateAct_, SIGNAL( triggered( ) ), this, SLOT( validateDialog( ) ) );

    transformAct_ = new QAction{tr( "XSLT Transformation" ), this};
    transformAct_->setStatusTip(
        tr( "Transform the current XML file using an XSLT stylesheet" ) );
    transformAct_->setIcon( QIcon::fromTheme( "document-export" ) );
    connect( transformAct_, SIGNAL( triggered( ) ), this, SLOT( transformDialog( ) ) );
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
}

QString Mainwindow::strippedName( const QString &fullFileName ) {
    return QFileInfo( fullFileName ).fileName( );
}

int Mainwindow::createNewOntologyViewTab( const QString &name, const QColor &color ) {
    OntologyView *ontologyView = new OntologyView;
    ontologyView->setContextMenuPolicy( Qt::CustomContextMenu );
    auto idx =
        tabOntologies_->insertTab( tabOntologies_->count( ) - 1, ontologyView, name );
    tabOntologies_->tabBar( )->setTabTextColor( idx, color );
    tabOntologies_->setCurrentIndex( idx );

    // connect( stackedTextDisplay_, &StackedTextDisplay::entrySelected, ontologyView,
    //          &OntologyView::insertEntry );
    connect( ontologyView, SIGNAL( synonymCreated( const QString &, const QString & )),
             ontologies_, SLOT( insertThesaurus( const QString &, const QString & )) );
    connect( ontologyView, SIGNAL( removeSynonym( const QString & )), ontologies_,
             SLOT( removeThesaurus( const QString & )) );
    connect( ontologyView, SIGNAL( removeEntry( const QString & )), ontologies_,
             SLOT( removeDictionary( const QString & )) );
    return idx;
}

void Mainwindow::deleteOntologyViewTab( int index ) {
    QWidget *widget = tabOntologies_->widget( index );
    tabOntologies_->removeTab( index );
    delete widget;
}

void Mainwindow::addEntryToCurrentTab( const QString &entry, int count ) {
    OntologyView *currentView =
        reinterpret_cast<OntologyView *>( tabOntologies_->currentWidget( ) );
    currentView->insertEntry( entry, count );
}

void Mainwindow::generateInitialSelection( const XmlElement &element ) {
    auto selection = ontologies_->generateSelection( element );
    selections_->addSelection( selection );
}
