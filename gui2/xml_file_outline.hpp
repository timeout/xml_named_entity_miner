#pragma once
#include <QTreeWidget>

class XmlDoc;
class XmlElement;
class QAction;
class QPoint;
class QTreeWidgetItem;

class XmlFileOutline : public QTreeWidget {
    Q_OBJECT
public:
    XmlFileOutline( QWidget *parent = nullptr );
    auto xml( const XmlDoc &xml ) -> void;
    auto writeXml( ) const -> XmlDoc;
    auto clear( ) -> void;

public slots:
    void itemClicked( const QModelIndex &idx );
    void xPathResult( const XmlElement &element );
    void clearSelections( );

signals:
    void xmlItemSelected( const XmlElement &element );
    void xmlItemDeselected( const XmlElement &element );

private slots:
    void addPositiveSentiment( );
    void addNegativeSentiment( );
    void addNeutralSentiment( );
    void customContextMenu( const QPoint &pos );

private:
    enum class Sentiment { Positive = 0x0, Neutral = 0x2, Negative = 0x4 };

    auto checkSubtree( QTreeWidgetItem *target ) -> void;
    auto uncheckSubtree( QTreeWidgetItem *target ) -> void;
    auto addSentiment( Sentiment sentiment ) -> void;
    auto contextMenu( ) -> void;
    auto connections( ) -> void;

    QMenu *fileOutlineContextMenu_;
    QAction *semanticAction_;
    QAction *positiveSemanticAction_;
    QAction *negativeSemanticAction_;
    QAction *neutralSemanticAction_;
    QTreeWidgetItem *selectedItem_;
};

