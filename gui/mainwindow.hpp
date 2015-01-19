#pragma once
#include <QMainWindow>

class QAction;
class XmlTree;
class XmlDisplay;
class QMenu;
class QPoint;
class QTabWidget;
class TxtSelectionDisplay;
class QToolBar;
class ElementSelections;

class Mainwindow : public QMainWindow {
    Q_OBJECT
public:
    Mainwindow( QWidget *parent = nullptr );

public slots:
    void open( );
    void onCustomContextRequest( const QPoint &pos );
    void onNextSelectionTriggered( );
    void onPreviousSelectionTriggered( );

private:
    void createMenus( );
    void createActions( );
    void loadXml( const QString &filename );

    // actions
    QAction *openAct_;

    QAction *nextSelectionAct_, *previousSelectionAct_;

    // menu
    QMenu *xmlTreeContextMenu_;

    // toolbar
    QToolBar *fileToolBar_, *documentToolBar_;

    // central widget(s)
    QTabWidget *tabWidget_;
    XmlDisplay *xmlDisplay_;
    TxtSelectionDisplay *txtSelectionDisplay_;

    ElementSelections *selections_;

    // docking
    XmlTree *xmlNavigator_;
};

