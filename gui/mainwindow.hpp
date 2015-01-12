#pragma once
#include "xml_tree.hpp"
#include "xml_doc.hpp"

#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QPoint>

#include <string>
#include <fstream>

class Mainwindow : public QMainWindow {
    Q_OBJECT
public:
    Mainwindow( QWidget *parent = nullptr )
        : QMainWindow{parent}, xml_{}, tr_{new XmlTree} {

        createActions( );
        createMenus( );
        setCentralWidget( tr_ );

        // context menu for tree view
        connect( tr_, SIGNAL( customContextMenuRequested( const QPoint & )), this,
                 SLOT( onCustomContextRequest( const QPoint & )) );
    }

public slots:
    void open( ) {
        const QString filename = QFileDialog::getOpenFileName(
            this, tr( "Open XML file" ), "/usr/local/home/joe",
            tr( "Xml Files (*.xml)" ) );
        if ( !filename.isEmpty( ) ) {
            std::ifstream in{filename.toUtf8( ).constData( )};
            xml_ = XmlDoc{in};
            tr_->xml( xml_ );
        }
    }
    void onCustomContextRequest( const QPoint &pos ) {
        QModelIndex idx = tr_->indexAt( pos );
        if ( idx.isValid( ) ) {
            xmlTreeContextMenu_->exec( tr_->mapToGlobal( pos ) );
        }
    }

private:
    void createMenus( ) {
        QMenu *fileMenu = menuBar( )->addMenu( tr( "&File" ) );
        fileMenu->addAction( openAct_ );

        xmlTreeContextMenu_ = new QMenu( this );
        // TODO: create actions: select all, clear selections?
        // stub
        xmlTreeContextMenu_->addAction( openAct_ );
    }

    void createActions( ) {
        openAct_ = new QAction{tr( "&Open" ), this};
        openAct_->setShortcuts( QKeySequence::Open );
        openAct_->setStatusTip( tr( "Open an existing file" ) );
        connect( openAct_, SIGNAL( triggered( ) ), this, SLOT( open( ) ) );
    }

    XmlDoc xml_;
    QAction *openAct_;
    XmlTree *tr_;
    QMenu *xmlTreeContextMenu_;
};

