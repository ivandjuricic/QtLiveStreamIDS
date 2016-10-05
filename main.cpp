#include "mainwindow.h"
#include <QApplication>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowFlags(Qt::WindowStaysOnTopHint |
                          Qt::CustomizeWindowHint);
    window.setGeometry(QRect(400,100,400,400)); // TO BE SET TO SAMPLE POSITION LATER

    window.show();

    return app.exec();
}
