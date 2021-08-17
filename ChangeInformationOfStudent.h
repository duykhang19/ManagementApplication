#ifndef CHANGEINFORMATIONOFSTUDENT_H
#define CHANGEINFORMATIONOFSTUDENT_H

#include <QWidget>
#include<QKeyEvent>
#include<QCloseEvent>
#include<QMessageBox>
#include<QPushButton>
#include<QDebug>
#include<QSqlQuery>
#include<QDate>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#include<QToolTip>
#include<QPalette>
#include<QFileDialog>
#include<QFileInfo>
#include<sstream>
#include<string>
#include<QString>
#include<cctype>
#include<iostream>
#include<string.h>
#include"Person.h"

namespace Ui {
class ChangeInformationOfStudent;
}

class ChangeInformationOfStudent : public QWidget, public Person
{
    Q_OBJECT
private:
    QString username;
    QString password;
    QString datebirth;
    QString placeofbirth;
    QString sex;
    QString ethnic;
    QString phonenumber;
    QString identitycard;
    QString address;
    QString email;
    int indexForDistinguish;
    QString id;
    Ui::ChangeInformationOfStudent *ui;

public:
    explicit ChangeInformationOfStudent(QWidget *parent = nullptr);
    ~ChangeInformationOfStudent();
    void getUserPass(QString user, QString pass);
    QString getplaceofbirth();
    QString getethnic();
    QString getaddress();

    bool checkValidPhoneNumber();
    bool checkValidIdentityCard();
    bool checkValidEmail();
    bool checkValidEthnic();
    bool checkValidAddress();
    bool checkValidPlaceOfBirth();
    void setDistinguish(int);
    int getDistinguish();
    void setIdd(QString);
    QString getIdd();
private slots:
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void on_pushButton_update_clicked();
    void on_lineEdit_date_editingFinished();
    //void on_lineEdit_phonenumber_editingFinished();
    //void on_lineEdit_cmnd_editingFinished();
    //void on_lineEdit_email_editingFinished();
    void on_lineEdit_placebirth_editingFinished();
    void on_lineEdit_ethnic_editingFinished();
    void on_lineEdit_address_editingFinished();
    void on_pushButton_addPic_clicked();
    void on_lineEdit_phonenumber_textChanged(const QString &arg1);
    void on_lineEdit_cmnd_textChanged(const QString &arg1);
    void on_lineEdit_email_textChanged(const QString &arg1);
    void on_pushButton_clicked();
};

#endif // CHANGEINFORMATIONOFSTUDENT_H
