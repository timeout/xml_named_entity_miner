#pragma once
#include <QStackedWidget>
#include <QMap>

class StackedTextDisplay : public QStackedWidget {
    Q_OBJECT
public:
    StackedTextDisplay( QWidget *parent = nullptr );

public slots:
    void clear( );
    void insertDisplay( int index, const QString &text );
    void removeDisplay( int index );
    void addOntology( const QColor &color );
    void removeOntology( int ontologyIndex );
    void insertHighlightRule( int index, int ontologyIndex, const QString &rule );
    void removeHighlightRule( int index, int ontologyIndex, const QString &rule );

signals:
    void entrySelected( int index, const QString &entry );

private slots:
    void addDisplayIndex( const QString &entry );
};
