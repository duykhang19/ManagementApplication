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

class ChangeInformationOfStudent : public QWidget,public Person
{
    Q_OBJECT
private:
    bool check;int b;QString oldID;
    Ui::ChangeInformationOfStudent *ui;

public:
    explicit ChangeInformationOfStudent(QWidget *parent = nullptr, int a = 0);
    void showLineEditClass();
    void hideLineEditClass();
    void setLName(QString);void setLMajor(QString);
    void setLId(QString);void setLClass(QString);
    void setOldId(QString);
    bool checkIdIsValid();
    void setB(int);
    ~ChangeInformationOfStudent();

private slots:
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void on_pushButton_update_clicked();

};

#endif // CHANGEINFORMATIONOFSTUDENT_H
