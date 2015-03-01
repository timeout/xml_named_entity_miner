#include "mainwindow.hpp"
#include "xml_doc.hpp"
#include "xml_display.hpp"
#include "stacked_text_display.hpp"
#include "xml_file_explorer.hpp"

#include <fstream>
#include <sstream>

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QStatusBar>
#include <QString>
#include <QTabWidget>
#include <QToolBar>

static auto strippedName( const QString &fullFileName ) -> QString;

struct MainWindow::Impl {
    Impl( );
    auto updateRecentFileActions( ) -> void;
    auto configureActions( ) -> void;
    auto configureMenu( ) -> void;
    auto configureFileToolBar( ) -> void;

    QAction *openAct_;
    QAction *exitAct_;
    QAction *recentFileActs_[MaxRecentFiles];
    QAction *nextAct_, *prevAct_;

    QMenu *fileMenu_, *viewMenu_;
    QToolBar *fileToolBar_, *viewToolBar_;

    QString curFile_;
    XmlDoc xml_;
};

MainWindow::Impl::Impl( )
    : openAct_{nullptr}
    , exitAct_{nullptr}
    , recentFileActs_{nullptr}
    , nextAct_{nullptr}
    , prevAct_{nullptr}
    , fileMenu_{nullptr}
    , viewMenu_{nullptr}
    , fileToolBar_{nullptr}
    , viewToolBar_{nullptr} {}

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
    nextAct_->setStatusTip( tr( "Activate next selection" ) );
    nextAct_->setIcon( QIcon::fromTheme( "arrow-right" ) );
    nextAct_->setEnabled( false );
    prevAct_->setStatusTip( tr( "Activate previous selection" ) );
    prevAct_->setIcon( QIcon::fromTheme( "arrow-left" ) );
    prevAct_->setEnabled( false );
}

auto MainWindow::Impl::configureMenu( ) -> void {
    fileMenu_->addAction( openAct_ );
    QMenu *recentFilesMenu = fileMenu_->addMenu( QIcon::fromTheme( "application-menu" ),
                                                 tr( "Recent &Files" ) );
    for ( int i = 0; i < MaxRecentFiles; ++i )
        recentFilesMenu->addAction( recentFileActs_[i] );
    fileMenu_->addAction( exitAct_ );
    updateRecentFileActions( );

    viewMenu_->addAction( nextAct_ );
    viewMenu_->addAction( prevAct_ );
}

// -- end Impl

MainWindow::MainWindow( )
    : QMainWindow{}
    , impl_{new Impl}
    , xmlDisplay_{new XmlDisplay{this}}
    , stackedTextDisplay_{new StackedTextDisplay{this}}
    , xmlFileExplorer_{new XmlFileExplorer{this}} {
    readSettings( );
    initActions( );
    initMenus( );
    initToolBar( );
    initCentralWidget( );
    initFileExplorer( ); // left dock widget
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
    const QString filename = QFileDialog::getOpenFileName(
        this, tr( "Open XML file" ), "/usr/local/home/joe", tr( "Xml Files (*.xml)" ) );
    if ( !filename.isEmpty( ) ) {
        loadFile( filename );
    }
}

auto MainWindow::openRecentFile( ) -> void {
    QAction *action = qobject_cast<QAction *>( sender( ) );
    if ( action ) {
        loadFile( action->data( ).toString( ) );
    }
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
    impl_->nextAct_ = new QAction{tr( "Next Selection" ), this};
    impl_->prevAct_ = new QAction{tr( "Previous Selection" ), this};
    impl_->configureActions( );
}

auto MainWindow::initMenus( ) -> void {
    impl_->fileMenu_ = menuBar( )->addMenu( tr( "&File" ) );
    impl_->viewMenu_ = menuBar( )->addMenu( tr( "&View" ) );
    impl_->configureMenu( );
}

auto MainWindow::initToolBar( ) -> void {
    impl_->fileToolBar_ = addToolBar( "ApplicationToolBar" );
    impl_->viewToolBar_ = addToolBar( "ApplicationToolBar" );
    impl_->configureFileToolBar( );
}

auto MainWindow::initCentralWidget( ) -> void {
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->addTab( xmlDisplay_, tr( "Overview" ) );
    tabWidget->addTab( stackedTextDisplay_, tr( "Selection" ) );
    tabWidget->setTabPosition( QTabWidget::South );
    setCentralWidget( tabWidget );
}

auto MainWindow::initFileExplorer( ) -> void {
    QDockWidget *leftDock = new QDockWidget( tr( "Xml File Explorer" ), this );
    leftDock->setObjectName( "LeftDock" );
    leftDock->setWidget( xmlFileExplorer_ );
    leftDock->setAllowedAreas( Qt::LeftDockWidgetArea );
    leftDock->setFeatures( QDockWidget::DockWidgetVerticalTitleBar );
    addDockWidget( Qt::LeftDockWidgetArea, leftDock );
}

auto MainWindow::initConnections( ) -> void {
    connect( impl_->openAct_, &QAction::triggered, this, &MainWindow::open );
    connect( impl_->exitAct_, &QAction::triggered, this, &MainWindow::close );
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

    connect( xmlFileExplorer_, &XmlFileExplorer::elementSelected, stackedTextDisplay_,
             &StackedTextDisplay::addElement );
    connect( xmlFileExplorer_, &XmlFileExplorer::elementDeselected, stackedTextDisplay_,
             &StackedTextDisplay::removeElement );
}

auto MainWindow::maybeSave( ) const -> bool { return true; }

auto MainWindow::loadFile( const QString &fileName ) -> void {
    std::ifstream in{fileName.toStdString( )};
    if ( in.fail( ) ) {
        QString error = QString::fromStdString( strerror( errno ) );
        QMessageBox::warning(
            this, tr( "Recent Files" ),
            tr( "Cannot read file %1:\n%2." ).arg( fileName ).arg( error ) );
        return;
    }

    impl_->xml_ = XmlDoc{in};
    XmlDoc &xml = impl_->xml_;
    if ( xml ) {
        // xml open and exists
        QApplication::setOverrideCursor( Qt::WaitCursor );
        // do stuff with the xml file
        // TODO:
        xmlDisplay_->setXml( xml );
        xmlFileExplorer_->setXml( xml );
        QApplication::restoreOverrideCursor( );

        setCurrentFile( fileName );
        statusBar( )->showMessage( tr( "File loaded" ), 2000 );
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

auto strippedName( const QString &fullFileName ) -> QString {
    return QFileInfo( fullFileName ).fileName( );
}

