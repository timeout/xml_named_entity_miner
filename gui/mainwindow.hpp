#pragma once
#include <QMainWindow>

class QAction;
class XmlTree;
class XmlDisplay;
class QMenu;
class QPoint;

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
    void loadXml(const QString& filename);

    QAction *openAct_;
    XmlDisplay *xmlDisplay_;
    XmlTree *xmlNavigator_;
    QMenu *xmlTreeContextMenu_;
};

