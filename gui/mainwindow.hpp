#pragma once
#include <QMainWindow>
#include "xml_doc.hpp"
#include "synonyms.hpp"

class QAction;
class XmlFileOutline;
class XmlDisplay;
class QMenu;
class QPoint;
class QTabWidget;
class StackedTextDisplay;
// class TxtSelectionDisplay;
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
    void closeTab( int idx );
    void addEntryToCurrentTab( const QString &entry, int count );
    void generateInitialSelection( const XmlElement &element );

private:
    void createMenus( );
    void createActions( );
    void loadFile( const QString &fileName );
    void updateRecentFileActions( );
    void setCurrentFile( const QString &fileName );
    QString strippedName( const QString &fullFileName );
    // void loadXml( const QString &filename );
    int createNewOntologyViewTab( const QString &name, const QColor &color );
    void deleteOntologyViewTab( int index );

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
    StackedTextDisplay *stackedTextDisplay_;
    // TxtSelectionDisplay *txtSelectionDisplay_;

    // docking
    XmlFileOutline *xmlFileOutline_;
    XPathQueryWidget *xPathQueryWidget_;
    QTabWidget *tabOntologies_;
    // Ontology *ontology_;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QString curFile_;

    Ontologies *ontologies_;
    ElementSelections *selections_;
    XmlDoc xml_;
};

