#pragma once
#include "xml_display.hpp"
#include "xml_tree.hpp"
#include "xml_doc.hpp"

#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QPoint>
#include <QDockWidget>
#include <QStatusBar>

#include <string>
#include <fstream>

class Mainwindow : public QMainWindow {
    Q_OBJECT
public:
    Mainwindow( QWidget *parent = nullptr );

public slots:
    void open( );
    void onCustomContextRequest( const QPoint &pos );

private:
    void createMenus( );
    void createActions( );

    XmlDoc xml_;
    QAction *openAct_;
    XmlDisplay *xmlDisplay_;
    XmlTree *xmlNavigator_;
    QMenu *xmlTreeContextMenu_;
};

