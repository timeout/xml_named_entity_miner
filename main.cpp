#include "lib/bilanz.hpp"
#include "gui/mainwindow.hpp"
#include <QApplication>

int main( int argc, char *argv[] ) {
    QApplication app( argc, argv );
    Mainwindow win;
    win.show( );
    return app.exec( );
}
