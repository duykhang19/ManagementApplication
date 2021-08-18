//#include "mainwindow.h"
#include <QApplication>
#include"connectToSqlServer.h"
#include<Student_Interactive_Interface.h>
#include<QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    connectToSqlServer();
//    MainWindow w;
//    w.show();
    a.setWindowIcon(QIcon(":/boy_studeent.png"));
    Student_Interactive_Interface s;
    s.showMaximized();
    return a.exec();
}
