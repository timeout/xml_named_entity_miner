#pragma once
#include <QMainWindow>
#include "xml_doc.hpp"

class QAction;
class XmlFileOutline;
class Ontology;
class XmlDisplay;
class QMenu;
class QPoint;
class QTabWidget;
class TxtSelectionDisplay;
class QToolBar;
class ElementSelections;
class XPathQueryWidget;

class Mainwindow : public QMainWindow {
    Q_OBJECT
public:
    Mainwindow( QWidget *parent = nullptr );

public slots:
    void open( );
    void onCustomContextRequest( const QPoint &pos );
    void onNextSelectionTriggered( );
    void onPreviousSelectionTriggered( );
    void openRecentFile( );
    void setNewOntology( );
    void validateDialog( );
    void transformDialog( );

private:
    void createMenus( );
    void createActions( );
    void loadFile( const QString &fileName );
    void updateRecentFileActions( );
    void setCurrentFile( const QString &fileName );
    QString strippedName( const QString &fullFileName );
    void loadXml( const QString &filename );

    // actions
    QAction *openAct_, *exitAct_;

    QAction *nextSelectionAct_, *previousSelectionAct_;
    QAction *validateAct_, *transformAct_;

    // menu
    QMenu *xmlTreeContextMenu_;
    QMenu *recentFilesMenu_;

    // toolbar
    QToolBar *fileToolBar_, *documentToolBar_;

    // central widget(s)
    QTabWidget *tabWidget_;
    XmlDisplay *xmlDisplay_;
    TxtSelectionDisplay *txtSelectionDisplay_;

    ElementSelections *selections_;

    // docking
    XmlFileOutline *xmlFileOutline_;
    XPathQueryWidget *xPathQueryWidget_;
    QTabWidget *tabOntologies_;
    Ontology *ontology_;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QString curFile_;

    XmlDoc xml_;
};

