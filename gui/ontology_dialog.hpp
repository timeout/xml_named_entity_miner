#pragma once
#include <QDialog>
#include <QColor>
#include <QLineEdit>

class QLabel;
class QComboBox;
class QPushButton;
class QDialogButtonBox;
class QGroupBox;

class OntologyDialogLineEdit : public QLineEdit {
    Q_OBJECT
public:
    OntologyDialogLineEdit( QWidget *parent = nullptr );

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

class OntologyDialog : public QDialog {
    Q_OBJECT
public:
    explicit OntologyDialog( QWidget *parent = nullptr );
    const QString name( ) const;
    const QColor color( ) const;

public slots:
    void startColourDialog( );

signals:
    void colorChanged( const QColor &color );
    void ontologyNameChanged( const QString &name );

private:
    void setChooseColor( );

    void layout( );
    void connections( );

    QGroupBox *configGroupBox_;
    QLabel *label_;
    QComboBox *ontologyName_;
    QPushButton *chooseColor_;
    QDialogButtonBox *buttonBox_;

    QColor color_;
    bool defaultEntryCleared_;
};
