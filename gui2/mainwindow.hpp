#pragma once
#include <QMainWindow>
#include <memory>

class QCloseEvent;

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

private:
    auto readSettings( ) -> void;
    auto writeSettings( ) -> void;
    auto initActions( ) -> void;
    auto initMenus( ) -> void;
    auto initToolBar( ) -> void;
    auto initConnections( ) -> void;
    auto maybeSave( ) const -> bool;
    auto loadFile( const QString &fileName ) -> void;
    auto setCurrentFile( const QString &fileName ) -> void;

    struct Impl;
    std::unique_ptr<Impl> impl_;
};
