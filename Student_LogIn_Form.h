#ifndef STUDENT_LOGIN_FORM_H
#define STUDENT_LOGIN_FORM_H

#include <QMainWindow>
#include<QString>
#include<QDate>
#include<QTimer>
#include<QMessageBox>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#include<QRegExp>
#include<QDebug>
#include<QToolTip>
#include<QPoint>
#include<QColor>
#include<QPalette>
#include"Lecturer_LogIn_Form.h"
#include<QDate>
#include<QKeyEvent>
#include"Person.h"
#include<QAction>
#include<QCursor>
#include<QToolButton>
#include<fstream>

namespace Ui {
class Student_LogIn_Form;
};

class Student_LogIn_Form : public QMainWindow,public Person
{
    Q_OBJECT

public:
    explicit Student_LogIn_Form(QWidget *parent = nullptr);
    ~Student_LogIn_Form();
private:
    QString MSSV;
    QString CLASSNAME;
    QString MAJOR;
    Ui::Student_LogIn_Form *ui;
    QTimer *timer;
    bool checkStudentID;
    bool checkClassname;
    bool checkMajor;
    QToolButton* button;
public:
    void getInformation() override;
    bool checkDataIsNull() override;
    void insertIntoDatabase() override;
    bool checkForDuplicateData() override;
    void doNothing() override;
    void closeWindow();
    bool checkClassExist(QString& classNamee);
    void setClassname(QString classname){this->CLASSNAME = classname;}
    QString getClassname(){return this->CLASSNAME;}
    void setCheckStudenId(bool check){this->checkStudentID= check;}
    void setCheckMajor(bool check){this->checkMajor = check;}
    void setMajor(QString profess){this->MAJOR = profess;}
    void setStudentId(QString id){this->MSSV = id;}
    QString getStudentId(){return this->MSSV;}
    QString getMajor(){return this->MAJOR;}
    bool getCheckStudentId(){return this->checkStudentID;}
    bool getCheckMajor(){return this->checkMajor;}
    void setCheckClassname(bool check){this->checkClassname = check;}
    bool getCheckClassname(){return this->checkClassname;}
    void editingStudentIDFinished();
    bool checkStudentIDExistInSystem();
    void initPasswordHint() override;
    void passwordShow() override;
    void passwordHint() override;
    void showButton();
    void hideButton();
private slots:   
    void UpdateTime();
    void on_lineEdit_phonenumbers_editingFinished();
    void on_lineEdit_emails_editingFinished();
    void on_lineEdit_cmnds_editingFinished();
    void on_pushButton_exits_clicked();
    void enterEventSignUp();
    void enterEventExit();
    void on_pushButton_sig_clicked();
    void keyReleaseEvent(QKeyEvent *event) override;
    void on_lineEdit_datebirths_editingFinished();
    void on_lineEdit_usernames_editingFinished();
    void on_lineEdit_addresss_editingFinished();
    void closeEvent(QCloseEvent *event) override;
    void on_lineEdit_mssvs_textChanged(const QString &arg1);
};

#endif // STUDENT_LOGIN_FORM_H
