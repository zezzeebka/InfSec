#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    if(window.isLoggedIn()){
    window.show();
    }
    else{
        return 0;
    }
    window.show();
    return app.exec();
}
