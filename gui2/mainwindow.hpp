#pragma once
#include <QMainWindow>
#include <memory>

class QCloseEvent;
class XmlDisplay;
class StackedTextDisplay;
class XmlFileExplorer;
class TabbedOntologyView;

constexpr int MaxRecentFiles{5};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow( );
    virtual ~MainWindow( );

protected:
    virtual auto closeEvent( QCloseEvent *event ) -> void override;

private slots:
    void open( );
    void openRecentFile( );
    bool save( );
    bool saveAs( );
    void textEditLockToggle( bool checked );
    void textEditLock( bool checked );
    void textEditUnlock( bool checked );
    void schemaValidationDialog( );
    void transformDialog( );
    void showXmlOverview( );
    void showTextDisplay( );
    void enableSelectionTab( bool enabled );

signals:
    void enableXmlTask( bool enable );
    void xmlLoaded( );

private:
    auto readSettings( ) -> void;
    auto writeSettings( ) -> void;
    auto initActions( ) -> void;
    auto initMenus( ) -> void;
    auto initToolBar( ) -> void;
    auto initCentralWidget( ) -> void;
    auto initFileExplorer( ) -> void;
    auto initTabbedOntologyView( ) -> void;
    auto initStatusBar( ) -> void;
    auto initConnections( ) -> void;
    auto loadFile( const QString &fileName ) -> void;
    auto setCurrentFile( const QString &fileName ) -> void;
    auto writeXml( const QString &fileName ) const -> bool;
    auto saveFile( const QString &fileName ) -> bool;
    auto maybeSave( ) -> bool;

    struct Impl;
    std::unique_ptr<Impl> impl_;

    XmlDisplay *xmlDisplay_;
    StackedTextDisplay *stackedTextDisplay_;
    XmlFileExplorer *xmlFileExplorer_;
    TabbedOntologyView *tabbedOntologyView_;
};
