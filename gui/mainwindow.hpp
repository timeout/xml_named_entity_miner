#pragma once
#include "xml_tree.hpp"
#include "xml_doc.hpp"

#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

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
    }

public slots:
    void open( ) {
        const QString filename = QFileDialog::getOpenFileName(
            this, tr( "Open XML file" ), "/usr/local/home/joe",
            tr( "Xml Files (*.xml)" ) );
        std::ifstream in{filename.toUtf8( ).constData( )};
        xml_ = XmlDoc{in};
        tr_->xml( xml_ );
    }

private:
    void createMenus( ) {
        QMenu *fileMenu = menuBar( )->addMenu( tr( "&File" ) );
        fileMenu->addAction( openAct_ );
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
};

