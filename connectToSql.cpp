#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include "connectToSqlServer.h"
#include<QMessageBox>
#include<QPushButton>
#include<QString>
#include<string>
#include<QAbstractButton>
#include<QObject>
#include<QNetworkAccessManager>
#include<QtNetwork>

void connectToSqlServer()
{
    if (!checkInternetConnection())
    {
        QMessageBox msg;
        msg.setStyleSheet("font : 6pt Courier;color : black");//:/icon-file/addMark_icon.png
        msg.setFixedSize(350,200);
        msg.setWindowTitle("Thông báo !");
        msg.setWindowIcon(QIcon(":/icon-file/jpg/waning.png"));
        QPushButton* buttonYes = msg.addButton(QObject::tr("Đồng ý"),QMessageBox::YesRole);
        buttonYes->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
        "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
        "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
        msg.setText("Kiểm tra lại kết nối Internet và khỏi động lại ứng dụng !");
        buttonYes->setFocusPolicy(Qt::NoFocus);
        msg.exec();
        if (msg.clickedButton() == buttonYes)
        {
            msg.close();
            exit(0);
        }
    }


    QSqlDatabase connectSqlToQt = QSqlDatabase::addDatabase("QODBC");
    connectSqlToQt.setHostName("DESKTOP-04RLSIC\\SQLEXPRESS");
//    connectSqlToQt.setDatabaseName("duykhan-connect");
    connectSqlToQt.setDatabaseName("connectToSql");
//    connectSqlToQt.setUserName("duykhan_");
//    connectSqlToQt.setPassword("123456");
    connectSqlToQt.setPort(1434);
    connectSqlToQt.setConnectOptions();
    if (connectSqlToQt.open())
    {
        qDebug() << "Connect to SQL Server Successfully !";
    }
    else
    {
        QMessageBox msg;
        msg.setStyleSheet("font : 6pt Courier;color : black");
        msg.setFixedSize(400,200);
        msg.setWindowTitle("Thông báo !");
        msg.setWindowIcon(QIcon(":/icon-file/jpg/waning.png"));
        QPushButton* buttonYes = msg.addButton(QObject::tr("Đồng ý"),QMessageBox::YesRole);
        buttonYes->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
        "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
        "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
        msg.setText("   Không thể kết nối đến hệ thống máy chủ.\t\n\tVui lòng thử lại sau giây lát !");
        buttonYes->setFocusPolicy(Qt::NoFocus);
        msg.exec();
        if (msg.clickedButton() == buttonYes)
        {

            msg.close();
            exit(0);
        }
    }
}

bool checkInternetConnection()
{

    QNetworkAccessManager man;
    QNetworkRequest reg(QUrl("http://www.google.com"));
    QNetworkReply *reply = man.get((reg));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    if(!reply->isFinished())
    loop.exec();
    if(reply->bytesAvailable())
    {
      return true;
    }
    return false;
};
