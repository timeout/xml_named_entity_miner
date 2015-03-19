#include "mainwindow.hpp"
#include "xml_doc.hpp"
#include "xml_display.hpp"
#include "stacked_text_display.hpp"
#include "xml_file_explorer.hpp"
#include "tabbed_ontology_view.hpp"
#include "text_display.hpp"
#include "thesaurus.hpp"
#include "xml_schema_dialog.hpp"
#include "xslt_dialog.hpp"

#include "xpath_ctxt.hpp"
#include "xpath_query.hpp"
#include "xpath_result_set.hpp"

#include <fstream>
#include <sstream>
#include <queue>

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>

static auto strippedName( const QString &fullFileName ) -> QString;

struct MainWindow::Impl {
    Impl( );
    auto updateRecentFileActions( ) -> void;
    auto configureActions( ) -> void;
    auto configureMenu( ) -> void;
    auto configureFileToolBar( ) -> void;
    auto configureStatusBar( ) -> void;
    auto toggleEditLockIcon( bool checked ) -> void;

    QAction *openAct_, *exitAct_;
    QAction *recentFileActs_[MaxRecentFiles];
    QAction *saveAct_, *saveAsAct_;
    QAction *nextAct_, *prevAct_;
    QAction *xmlViewAct_, *textViewAct_;
    QActionGroup *lockTextActionGroup_;
    QAction *unlockTextAct_, *lockTextAct_, *addOntologyAct_, *validateAct_,
        *transformAct_;

    QMenu *fileMenu_, *viewMenu_, *toolMenu_;
    QToolBar *fileToolBar_, *viewToolBar_;
    QToolButton *textEditLock_;
    QLabel *semanticLabel_;
    QTabWidget *tabbedWidget_;

    QString curFile_, saveFile_;
    XmlDoc xml_;
};

MainWindow::Impl::Impl( )
    : openAct_{nullptr}
    , exitAct_{nullptr}
    , recentFileActs_{nullptr}
    , saveAct_{nullptr}
    , saveAsAct_{nullptr}
    , nextAct_{nullptr}
    , prevAct_{nullptr}
    , xmlViewAct_{nullptr}
    , textViewAct_{nullptr}
    , lockTextActionGroup_{nullptr}
    , unlockTextAct_{nullptr}
    , lockTextAct_{nullptr}
    , addOntologyAct_{nullptr}
    , validateAct_{nullptr}
    , transformAct_{nullptr}
    , fileMenu_{nullptr}
    , viewMenu_{nullptr}
    , fileToolBar_{nullptr}
    , viewToolBar_{nullptr}
    , textEditLock_{nullptr}
    , semanticLabel_{nullptr}
    , tabbedWidget_{nullptr} {}

auto MainWindow::Impl::updateRecentFileActions( ) -> void {
    QSettings settings;
    QStringList files = settings.value( "recentFileList" ).toStringList( );

    int numRecentFiles = qMin( files.size( ), ( int )MaxRecentFiles );

    for ( int i = 0; i < numRecentFiles; ++i ) {
        QString text = tr( "&%1 %2" ).arg( i + 1 ).arg( strippedName( files[i] ) );
        recentFileActs_[i]->setText( text );
        recentFileActs_[i]->setData( files[i] );
        recentFileActs_[i]->setVisible( true );
    }
    for ( int j = numRecentFiles; j < MaxRecentFiles; ++j )
        recentFileActs_[j]->setVisible( false );
}

auto MainWindow::Impl::configureFileToolBar( ) -> void {
    fileToolBar_->setObjectName( "FileToolBar" );
    fileToolBar_->addAction( openAct_ );
    fileToolBar_->addAction( saveAct_ );
    fileToolBar_->addAction( exitAct_ );
    fileToolBar_->setMovable( false );
    fileToolBar_->setAllowedAreas( Qt::TopToolBarArea );
    fileToolBar_->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    viewToolBar_->setObjectName( "ViewToolBar" );
    viewToolBar_->addAction( nextAct_ );
    viewToolBar_->addAction( prevAct_ );
    viewToolBar_->setMovable( false );
    viewToolBar_->setAllowedAreas( Qt::TopToolBarArea );
    viewToolBar_->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
}

auto MainWindow::Impl::configureActions( ) -> void {
    openAct_->setShortcuts( QKeySequence::Open );
    openAct_->setStatusTip( tr( "Open an existing file" ) );
    openAct_->setIcon( QIcon::fromTheme( "document-open" ) );
    exitAct_->setShortcuts( QKeySequence::Quit );
    exitAct_->setStatusTip( tr( "Exit the application" ) );
    exitAct_->setIcon( QIcon::fromTheme( "application-exit" ) );
    saveAct_->setShortcut( QKeySequence::Save );
    saveAct_->setStatusTip( tr( "Save file" ) );
    saveAct_->setIcon( QIcon::fromTheme( "document-save" ) );
    saveAsAct_->setShortcut( QKeySequence::SaveAs );
    saveAsAct_->setStatusTip( tr( "Save file as..." ) );
    saveAsAct_->setIcon( QIcon::fromTheme( "document-save-as" ) );
    nextAct_->setStatusTip( tr( "Activate next selection" ) );
    nextAct_->setIcon( QIcon::fromTheme( "arrow-right" ) );
    nextAct_->setEnabled( false );
    prevAct_->setStatusTip( tr( "Activate previous selection" ) );
    prevAct_->setIcon( QIcon::fromTheme( "arrow-left" ) );
    prevAct_->setEnabled( false );
    xmlViewAct_->setStatusTip( tr( "Overview of the current XML document" ) );
    xmlViewAct_->setIcon( QIcon::fromTheme( "edit-find" ) );
    textViewAct_->setStatusTip( tr( "View Selected XML element's contents" ) );
    textViewAct_->setIcon( QIcon::fromTheme( "edit-find-replace" ) );
    textViewAct_->setEnabled( false );

    unlockTextAct_->setShortcut( QKeySequence{Qt::CTRL + Qt::Key_U} );
    unlockTextAct_->setCheckable( true );
    unlockTextAct_->setIcon( QIcon::fromTheme( "lock-insecure" ) );
    unlockTextAct_->setStatusTip( tr( "Unlock current text display panel for editing" ) );
    lockTextAct_->setShortcut( QKeySequence{Qt::CTRL + Qt::Key_L} );
    lockTextAct_->setCheckable( true );
    lockTextAct_->setIcon( QIcon::fromTheme( "lock-secure" ) );
    lockTextAct_->setStatusTip( tr( "Lock current text display panel for editing" ) );

    lockTextActionGroup_->setEnabled( false );
    lockTextActionGroup_->addAction( unlockTextAct_ );
    lockTextActionGroup_->addAction( lockTextAct_ );
    lockTextAct_->setChecked( true );

    addOntologyAct_->setIcon( QIcon::fromTheme( "bookmark-new" ) );
    addOntologyAct_->setStatusTip( tr( "Add an ontology" ) );
    validateAct_->setShortcut( QKeySequence{Qt::CTRL + Qt::Key_V} );
    validateAct_->setIcon( QIcon::fromTheme( "ok" ) );
    validateAct_->setStatusTip( "Validate XML using XML Schema file" );
    validateAct_->setEnabled( false );
    transformAct_->setShortcut( QKeySequence{Qt::CTRL + Qt::Key_T} );
    transformAct_->setIcon( QIcon::fromTheme( "media-playlist-shuffle" ) );
    transformAct_->setStatusTip( "Transform XML using XSLT stylesheet" );
    transformAct_->setEnabled( false );
}

auto MainWindow::Impl::configureMenu( ) -> void {
    fileMenu_->addAction( openAct_ );
    QMenu *recentFilesMenu = fileMenu_->addMenu( QIcon::fromTheme( "application-menu" ),
                                                 tr( "Recent &Files" ) );
    for ( int i = 0; i < MaxRecentFiles; ++i )
        recentFilesMenu->addAction( recentFileActs_[i] );
    fileMenu_->addAction( saveAct_ );
    fileMenu_->addAction( saveAsAct_ );
    fileMenu_->addAction( exitAct_ );
    updateRecentFileActions( );

    viewMenu_->addAction( xmlViewAct_ );
    viewMenu_->addAction( textViewAct_ );
    viewMenu_->addAction( nextAct_ );
    viewMenu_->addAction( prevAct_ );

    toolMenu_->addAction( lockTextAct_ );
    toolMenu_->addAction( unlockTextAct_ );
    toolMenu_->addSeparator( );
    toolMenu_->addAction( addOntologyAct_ );
    toolMenu_->addSeparator( );
    toolMenu_->addAction( validateAct_ );
    toolMenu_->addAction( transformAct_ );
}

auto MainWindow::Impl::configureStatusBar( ) -> void {
    textEditLock_ = new QToolButton;
    textEditLock_->setIcon( QIcon::fromTheme( "lock-secure" ) );
    textEditLock_->setMinimumSize( textEditLock_->sizeHint( ) );
    textEditLock_->setEnabled( false );
    textEditLock_->setCheckable( true );
    semanticLabel_ = new QLabel;
    semanticLabel_->setIndent( 3 );
}

auto MainWindow::Impl::toggleEditLockIcon( bool checked ) -> void {
    if ( checked ) {
        textEditLock_->setIcon( QIcon::fromTheme( "lock-insecure" ) );
        unlockTextAct_->setChecked( true );
    } else {
        textEditLock_->setIcon( QIcon::fromTheme( "lock-secure" ) );
        lockTextAct_->setChecked( true );
    }
}

// -- end Impl

MainWindow::MainWindow( )
    : QMainWindow{}
    , impl_{new Impl}
    , xmlDisplay_{new XmlDisplay{this}}
    , stackedTextDisplay_{new StackedTextDisplay{this}}
    , xmlFileExplorer_{new XmlFileExplorer{this}}
    , tabbedOntologyView_{new TabbedOntologyView{this}} {
    readSettings( );
    initActions( );
    initMenus( );
    initToolBar( );
    initCentralWidget( );
    initFileExplorer( ); // left dock widget
    initTabbedOntologyView( );
    initStatusBar( );
    initConnections( );
}

MainWindow::~MainWindow( ) = default;

auto MainWindow::closeEvent( QCloseEvent *event ) -> void {
    if ( maybeSave( ) ) {
        writeSettings( );
        event->accept( );
    } else {
        event->ignore( );
    }
}

auto MainWindow::open( ) -> void {
    if ( maybeSave( ) ) {
        const QString filename = QFileDialog::getOpenFileName(
            this, tr( "Open XML file" ), "/usr/local/home/joe",
            tr( "Xml Files (*.xml)" ) );
        if ( !filename.isEmpty( ) ) {
            loadFile( filename );
        }
    }
}

auto MainWindow::openRecentFile( ) -> void {
    QAction *action = qobject_cast<QAction *>( sender( ) );
    if ( action ) {
        if ( maybeSave( ) ) {
            loadFile( action->data( ).toString( ) );
        }
    }
}

auto MainWindow::save( ) -> bool {
    if ( impl_->saveFile_.isEmpty( ) ) {
        return saveAs( );
    } else {
        return saveFile( impl_->saveFile_ );
    }
}

auto MainWindow::saveAs( ) -> bool {
    QFileDialog dialog( this );
    dialog.setWindowTitle( tr( "Save file" ) );
    dialog.setWindowModality( Qt::WindowModal );
    dialog.setAcceptMode( QFileDialog::AcceptSave );
    dialog.setNameFilter( tr( "XML Files (*.xml);; NEX Files (*nex)" ) );
    QStringList files;
    if ( dialog.exec( ) )
        files = dialog.selectedFiles( );
    else
        return false;

    return saveFile( files.at( 0 ) );
}

void MainWindow::textEditLockToggle( bool checked ) {
    impl_->toggleEditLockIcon( checked );
    if ( checked ) {
        impl_->unlockTextAct_->trigger( );
    } else {
        impl_->lockTextAct_->trigger( );
    }
}

void MainWindow::textEditLock( bool checked ) {
    if ( !checked && impl_->textEditLock_->isChecked( ) ) {
        impl_->toggleEditLockIcon( true );
    }
}

void MainWindow::textEditUnlock( bool checked ) {
    if ( checked && !impl_->textEditLock_->isChecked( ) ) {
        impl_->toggleEditLockIcon( true );
    }
}

void MainWindow::schemaValidationDialog( ) {
    // TODO:
    XmlSchemaDialog dialog{impl_->xml_, this};
    dialog.exec( );
}

void MainWindow::transformDialog( ) {
    // TODO:
    XsltDialog dialog{impl_->xml_, this};
    if ( dialog.exec( ) ) {
        if ( maybeSave( ) ) {
            impl_->xml_ = dialog.transformedXml( );
            QApplication::setOverrideCursor( Qt::WaitCursor );
            stackedTextDisplay_->clear( );
            xmlDisplay_->setXml( impl_->xml_ );
            xmlFileExplorer_->setXml( impl_->xml_ );
            QApplication::restoreOverrideCursor( );

            statusBar( )->showMessage( tr( "File Transformed" ), 2000 );
            emit enableXmlTask( true );
        }
    }
}

void MainWindow::showXmlOverview( ) { impl_->tabbedWidget_->setCurrentIndex( 0 ); }

void MainWindow::showTextDisplay( ) { impl_->tabbedWidget_->setCurrentIndex( 1 ); }

void MainWindow::enableSelectionTab( bool enabled ) {
    if ( enabled == false ) {
        impl_->tabbedWidget_->setCurrentIndex( 0 );
    } else {
        impl_->tabbedWidget_->setCurrentIndex( 1 );
    }
    impl_->tabbedWidget_->setTabEnabled( 1, enabled );
}

auto MainWindow::readSettings( ) -> void {
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    resize( settings.value( "size", QSize( 400, 400 ) ).toSize( ) );
    move( settings.value( "pos", QPoint( 200, 200 ) ).toPoint( ) );
    restoreState( settings.value( "windowState" ).toByteArray( ) );
    settings.endGroup( );
}

auto MainWindow::writeSettings( ) -> void {
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    settings.setValue( "size", size( ) );
    settings.setValue( "pos", pos( ) );
    settings.setValue( "windowState", saveState( ) );
    settings.endGroup( );
}

auto MainWindow::initActions( ) -> void {
    impl_->openAct_ = new QAction{tr( "&Open" ), this};
    impl_->exitAct_ = new QAction{tr( "E&xit" ), this};
    for ( int i = 0; i < MaxRecentFiles; ++i ) {
        impl_->recentFileActs_[i] = new QAction{this};
        impl_->recentFileActs_[i]->setVisible( true );
        connect( impl_->recentFileActs_[i], &QAction::triggered, this,
                 &MainWindow::openRecentFile );
    }
    impl_->saveAct_ = new QAction{tr( "&Save" ), this};
    impl_->saveAsAct_ = new QAction{tr( "&Save As..." ), this};
    impl_->nextAct_ = new QAction{tr( "Next Selection" ), this};
    impl_->prevAct_ = new QAction{tr( "Previous Selection" ), this};
    impl_->xmlViewAct_ = new QAction{tr( "XML Overview" ), this};
    impl_->textViewAct_ = new QAction{tr( "Selection Text" ), this};
    impl_->lockTextActionGroup_ = new QActionGroup{this};
    impl_->unlockTextAct_ = new QAction{tr( "Unlock Text" ), this};
    impl_->lockTextAct_ = new QAction{tr( "Lock Text" ), this};
    impl_->addOntologyAct_ = new QAction{tr( "Add Ontology" ), this};
    impl_->validateAct_ = new QAction{tr( "Validate XML" ), this};
    impl_->transformAct_ = new QAction{tr( "Transform XML" ), this};
    impl_->configureActions( );
}

auto MainWindow::initMenus( ) -> void {
    impl_->fileMenu_ = menuBar( )->addMenu( tr( "&File" ) );
    impl_->viewMenu_ = menuBar( )->addMenu( tr( "&View" ) );
    impl_->toolMenu_ = menuBar( )->addMenu( tr( "&Tools" ) );
    impl_->configureMenu( );
}

auto MainWindow::initToolBar( ) -> void {
    impl_->fileToolBar_ = addToolBar( "ApplicationToolBar" );
    impl_->viewToolBar_ = addToolBar( "ApplicationToolBar" );
    impl_->configureFileToolBar( );
}

auto MainWindow::initCentralWidget( ) -> void {
    impl_->tabbedWidget_ = new QTabWidget;
    impl_->tabbedWidget_->addTab( xmlDisplay_, tr( "Overview" ) );
    auto index = impl_->tabbedWidget_->addTab( stackedTextDisplay_, tr( "Selection" ) );
    std::cerr << "selection index: " << index << std::endl;
    impl_->tabbedWidget_->setTabEnabled( index, false );
    impl_->tabbedWidget_->setTabPosition( QTabWidget::South );
    setCentralWidget( impl_->tabbedWidget_ );
}

auto MainWindow::initFileExplorer( ) -> void {
    QDockWidget *leftDock = new QDockWidget{this};
    leftDock->setObjectName( "LeftDock" );
    leftDock->setWidget( xmlFileExplorer_ );
    leftDock->setAllowedAreas( Qt::LeftDockWidgetArea );
    leftDock->setFeatures( QDockWidget::NoDockWidgetFeatures );
    leftDock->setTitleBarWidget( new QWidget{this} );
    addDockWidget( Qt::LeftDockWidgetArea, leftDock );
}

auto MainWindow::initTabbedOntologyView( ) -> void {
    QDockWidget *rightDock = new QDockWidget{this};
    rightDock->setObjectName( "RightDock" );
    rightDock->setWidget( tabbedOntologyView_ );
    rightDock->setAllowedAreas( Qt::RightDockWidgetArea );
    rightDock->setFeatures( QDockWidget::NoDockWidgetFeatures );
    rightDock->setTitleBarWidget( new QWidget{this} );
    addDockWidget( Qt::RightDockWidgetArea, rightDock );
}

auto MainWindow::initStatusBar( ) -> void {
    impl_->configureStatusBar( );
    statusBar( )->addWidget( impl_->textEditLock_ );
    statusBar( )->addWidget( impl_->semanticLabel_ );
    // updateStatusBar( );
}

auto MainWindow::initConnections( ) -> void {
    connect( impl_->openAct_, &QAction::triggered, this, &MainWindow::open );
    connect( impl_->saveAct_, &QAction::triggered, this, &MainWindow::save );
    connect( impl_->saveAsAct_, &QAction::triggered, this, &MainWindow::saveAs );
    connect( impl_->exitAct_, &QAction::triggered, this, &MainWindow::close );
    connect( impl_->addOntologyAct_, &QAction::triggered, tabbedOntologyView_,
             &TabbedOntologyView::addOntology );

    connect( impl_->xmlViewAct_, &QAction::triggered, this,
             &MainWindow::showXmlOverview );
    connect( impl_->textViewAct_, &QAction::triggered, this,
             &MainWindow::showTextDisplay );
    connect( stackedTextDisplay_, &StackedTextDisplay::hasContent, this,
             &MainWindow::enableSelectionTab );
    connect( this, &MainWindow::enableXmlTask, impl_->textViewAct_,
             &QAction::setEnabled );
    connect( this, &MainWindow::xmlLoaded, tabbedOntologyView_,
             &TabbedOntologyView::clearOntologyViews );
    // next controllers
    connect( impl_->nextAct_, &QAction::triggered, stackedTextDisplay_,
             &StackedTextDisplay::next );
    connect( stackedTextDisplay_, &StackedTextDisplay::enableNext, impl_->nextAct_,
             &QAction::setEnabled );
    // prev controllers
    connect( impl_->prevAct_, &QAction::triggered, stackedTextDisplay_,
             &StackedTextDisplay::prev );
    connect( stackedTextDisplay_, &StackedTextDisplay::enablePrev, impl_->prevAct_,
             &QAction::setEnabled );
    // edit text
    connect( impl_->unlockTextAct_, &QAction::triggered, stackedTextDisplay_,
             &StackedTextDisplay::unlock );
    connect( impl_->unlockTextAct_, &QAction::triggered, this,
             &MainWindow::textEditUnlock );
    connect( impl_->lockTextAct_, &QAction::triggered, stackedTextDisplay_,
             &StackedTextDisplay::lock );
    connect( impl_->lockTextAct_, &QAction::triggered, this, &MainWindow::textEditLock );
    connect( this, &MainWindow::enableXmlTask, impl_->validateAct_,
             &QAction::setEnabled );
    connect( impl_->validateAct_, &QAction::triggered, this,
             &MainWindow::schemaValidationDialog );
    connect( this, &MainWindow::enableXmlTask, impl_->transformAct_,
             &QAction::setEnabled );
    connect( impl_->transformAct_, &QAction::triggered, this,
             &MainWindow::transformDialog );

    connect( xmlFileExplorer_, &XmlFileExplorer::elementSelected, stackedTextDisplay_,
             &StackedTextDisplay::addElement );
    connect( xmlFileExplorer_, &XmlFileExplorer::elementDeselected, stackedTextDisplay_,
             &StackedTextDisplay::removeElement );

    // stacked text display senders
    connect( stackedTextDisplay_, &StackedTextDisplay::textAdded, tabbedOntologyView_,
             &TabbedOntologyView::scanText );
    connect( stackedTextDisplay_, &StackedTextDisplay::entitySelected,
             tabbedOntologyView_, &TabbedOntologyView::addEntity );

    connect( stackedTextDisplay_, &StackedTextDisplay::hasContent,
             impl_->lockTextActionGroup_, &QActionGroup::setEnabled );
    connect( stackedTextDisplay_, &StackedTextDisplay::hasContent, impl_->textEditLock_,
             &QToolButton::setEnabled );

    connect( impl_->textEditLock_, &QToolButton::toggled, this,
             &MainWindow::textEditLockToggle );

    connect( tabbedOntologyView_, &TabbedOntologyView::ontologyAdded, stackedTextDisplay_,
             &StackedTextDisplay::addOntology );
    connect( tabbedOntologyView_, &TabbedOntologyView::ontologyRemoved,
             stackedTextDisplay_, &StackedTextDisplay::removeOntology );
    connect( tabbedOntologyView_, &TabbedOntologyView::entityAdded, stackedTextDisplay_,
             &StackedTextDisplay::highlight );
    connect( tabbedOntologyView_, &TabbedOntologyView::entityRemoved, stackedTextDisplay_,
             &StackedTextDisplay::removeHighlight );
}

auto MainWindow::maybeSave( ) -> bool {
    if ( stackedTextDisplay_->isWindowModified( ) ) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(
            this, tr( "Application" ), tr( "The document has been modified.\n"
                                           "Do you want to save your changes?" ),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        if ( ret == QMessageBox::Save )
            return save( );
        else if ( ret == QMessageBox::Cancel )
            return false;
    }
    return true;
}

auto MainWindow::loadFile( const QString &fileName ) -> void {
    std::ifstream in{fileName.toStdString( )};
    if ( in.fail( ) ) {
        QString error = QString::fromStdString( strerror( errno ) );
        QMessageBox::warning(
            this, tr( "Recent Files" ),
            tr( "Cannot read file %1:\n%2." ).arg( fileName ).arg( error ) );
        return;
    }

    QApplication::setOverrideCursor( Qt::WaitCursor );
    impl_->xml_ = XmlDoc{in};
    QApplication::restoreOverrideCursor( );
    XmlDoc &xml = impl_->xml_;
    if ( xml ) {
        // xml open and exists
        QApplication::setOverrideCursor( Qt::WaitCursor );
        // do stuff with the xml file
        // TODO:
        stackedTextDisplay_->clear( );
        xmlDisplay_->setXml( xml );
        xmlFileExplorer_->setXml( xml );
        QApplication::restoreOverrideCursor( );

        impl_->saveFile_.clear( );
        setCurrentFile( fileName );
        statusBar( )->showMessage( tr( "File loaded" ), 2000 );
        emit enableXmlTask( true );
        emit xmlLoaded( );
    } else {
        QString msg;
        if ( xml.errorHandler( ).hasErrors( ) ) {
            std::ostringstream errors;
            errors << xml.errorHandler( );
            std::cerr << "Error handler: " << xml.errorHandler( ) << std::endl;
            msg = QString::fromStdString( errors.str( ) );
        } else {
            msg = "Could not create XML document";
        }
        QMessageBox::warning(
            this, tr( "Could not open XML" ),
            tr( "Could not open XML file %1:\n%2" ).arg( fileName ).arg( msg ) );
    }
}

auto MainWindow::setCurrentFile( const QString &fileName ) -> void {
    impl_->curFile_ = fileName;
    setWindowFilePath( impl_->curFile_ );

    QSettings settings;
    QStringList files = settings.value( "recentFileList" ).toStringList( );
    files.removeAll( fileName );
    files.prepend( fileName );
    while ( files.size( ) > MaxRecentFiles ) {
        files.removeLast( );
    }
    settings.setValue( "recentFileList", files );

    foreach ( QWidget *widget, QApplication::topLevelWidgets( ) ) {
        MainWindow *mainWin = qobject_cast<MainWindow *>( widget );
        if ( mainWin )
            mainWin->impl_->updateRecentFileActions( );
    }
}

auto MainWindow::writeXml( const QString &fileName ) const -> bool {
    QApplication::setOverrideCursor( Qt::WaitCursor );

    std::cerr << "write xml. . ." << std::endl;
    auto xml = xmlFileExplorer_->writeXml( );
    // std::cerr << xml << std::endl;

    XPathCtxt ctxt{xml};
    if ( ctxt.errorHandler( ).hasErrors( ) ) {
        // TODO:
        std::cerr << "error handler has errors" << std::endl;
    }
    XPathQuery xpq{ctxt};
    xpq.query( "//*[@checked=\"yes\"]" );
    if ( xpq ) {
        XPathResultSet resultSet{xpq};
        auto index = 0;
        for ( auto iter = resultSet.begin( ); iter != resultSet.end( );
              ++iter, ++index ) {
            std::cerr << *iter << std::endl;

            std::cerr << "index: " << index << " from: " << stackedTextDisplay_->count( )
                      << std::endl;

            auto textDisplay =
                reinterpret_cast<TextDisplay *>( stackedTextDisplay_->widget( index ) );
            auto element = textDisplay->xmlElement( );

            // std::cerr << element << std::endl;

            if ( element.hasChild( ) ) {
                for ( auto child = element.child( ); child; child = child.sibling( ) ) {
                    auto keyword = child.content( );
                    auto ontologyName = child.attribute( "type" );
                    if ( !ontologyName.empty( ) ) {
                        auto thes = tabbedOntologyView_->thesaurus(
                            QString::fromStdString( ontologyName ) );
                        auto canon = thes.canonical( QString::fromStdString( keyword ) );
                        if ( !canon.isEmpty( ) ) {
                            child.attribute( "synonym", canon.toStdString( ) );
                        }
                    }
                }
            }
            auto result = *iter;
            result.clear( );
            result.copy( element );
            result.removeAttribute( "checked" );
        }
    } else {
        std::cerr << "xpath query error" << std::endl;
    }

    std::ofstream out;
    out.open( fileName.toStdString( ) );
    if ( out.fail( ) ) {
        // TODO: error
        return false;
    }

    out << xml << std::endl;

    QApplication::restoreOverrideCursor( );
    return true;
}

auto MainWindow::saveFile( const QString &fileName ) -> bool {
    QFile file( fileName );
    if ( !file.open( QFile::WriteOnly | QFile::Text ) ) {
        QMessageBox::warning( this, tr( "Application" ),
                              tr( "Cannot write file %1:\n%2." ).arg( fileName ).arg(
                                  file.errorString( ) ) );
        return false;
    }

    auto success = writeXml( fileName );
    if ( !success ) {
        return false;
    }
    stackedTextDisplay_->setWindowModified( false );

    impl_->saveFile_ = fileName;
    setCurrentFile( fileName );
    statusBar( )->showMessage( tr( "File saved" ), 2000 );
    return true;
}

auto strippedName( const QString &fullFileName ) -> QString {
    return QFileInfo( fullFileName ).fileName( );
}

