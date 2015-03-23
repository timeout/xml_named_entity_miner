#include "gui2/mainwindow.hpp"
#include <QApplication>

int main( int argc, char *argv[] ) {
    QApplication app( argc, argv );
    app.setOrganizationName("University-of-Konstanz");
    app.setApplicationName("Named-Entity-Miner");
    // Mainwindow win;
    MainWindow win;
    win.show( );
    return app.exec( );
}
