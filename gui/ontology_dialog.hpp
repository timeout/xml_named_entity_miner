#pragma once
#include <QDialog>
#include <QColor>

class QLabel;
class QComboBox;
class QPushButton;
class QDialogButtonBox;
class QGroupBox;

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
