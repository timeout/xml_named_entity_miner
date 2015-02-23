#include "lib/bilanz.hpp"
// #include "gui/mainwindow.hpp"
#include "gui2/mainwindow.hpp"
#include <QApplication>

int main( int argc, char *argv[] ) {
    QApplication app( argc, argv );
    // Mainwindow win;
    MainWindow win;
    win.show( );
    return app.exec( );
}
