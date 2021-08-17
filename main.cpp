#include"mainwindow.h"
#include"connectToSqlServer.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    connectToSqlServer();
    w.show();
    return a.exec();
}
