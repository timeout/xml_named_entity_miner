#pragma once
#include <QLineEdit>
#include <QPalette>

class QFocusEvent;
class QKeyEvent;
class QMouseEvent;

class LineEdit : public QLineEdit {
    Q_OBJECT
public:
    LineEdit( QWidget *parent = nullptr );

public slots:
    void clearDefaultEntry( bool );

signals:
    void focussed( bool hasFocus );
    void keyPressed( bool wasPressed );
    void mousePressed( bool wasPressed );

protected:
    virtual void focusInEvent( QFocusEvent *event );
    virtual void keyPressEvent( QKeyEvent *event );
    virtual void mousePressEvent( QMouseEvent *event );

private:
    QPalette activePalette_;
    QPalette inactivePalette_;
    bool edited_;
};


