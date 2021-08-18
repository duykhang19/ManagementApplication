#include "Person.h"

//constructor
Person::Person(){}

void Person:: messageBoxOneButton(QString title,QString text, QString textButton, int w, int h,int wb,int hb)
{
    QMessageBox mess;
    mess.setFixedSize(w,h);
    mess.setText(text);
    mess.setWindowIcon(QIcon(":/icon-file/waning.png"));
    mess.setWindowTitle(title);
    mess.setStyleSheet("font:8pt Courier;background-color:white;padding-bottom:5px;");

    QPushButton* buttonYes = mess.addButton(textButton,QMessageBox::YesRole);
    buttonYes->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline:0}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}"
    "QPushButton:pressed{background-color: rgb(255, 170, 0);border:1px solid rgb(68, 68, 68);outline : 0;");
    buttonYes->setFocusPolicy(Qt::NoFocus);
    buttonYes->setFixedSize(wb,hb);

    mess.exec();
    if (mess.clickedButton() == buttonYes){mess.close();}
}

bool Person::messageBoxTwoButton(QString title,QString text, QString textButton1,QString textButton2, int w, int h,int wb,int hb)
{
    QMessageBox mes;
    mes.setWindowTitle(title);
    mes.setText(text);
    mes.setWindowIcon(QIcon(":/icon-file/waning.png"));
    mes.setFixedSize(w,h);
    mes.setStyleSheet("font : 8pt Courier");
    QPushButton* buttonYes = mes.addButton(textButton1,QMessageBox::YesRole);
    QPushButton* buttonNo = mes.addButton(textButton2,QMessageBox::NoRole);
    buttonYes->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
    buttonNo->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
    buttonYes->setFixedSize(wb,hb);
    buttonNo->setFixedSize(wb,hb);
    buttonYes->setFocusPolicy(Qt::NoFocus);
    buttonNo->setFocusPolicy(Qt::NoFocus);
    mes.exec();
    if(mes.clickedButton() == buttonYes){mes.close();return true;}
    return false;
}

//destructor
Person::~Person(){}
