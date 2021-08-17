#ifndef LECTURER_LOGIN_FORM_H
#define LECTURER_LOGIN_FORM_H
#include<QMessageBox>
#include <QMainWindow>
#include<QDate>
#include<QTimer>
#include<QMouseEvent>
#include<QLineEdit>
#include<string>
#include<QCalendarWidget>
#include<QSqlResult>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#include<QToolTip>
#include<QDate>
#include<QPoint>
#include<QKeyEvent>
#include"Person.h"
#include<QAction>
#include<QToolButton>

using namespace std;

namespace Ui {
class Lecturer_LogIn_Form;
};

class Lecturer_LogIn_Form : public QMainWindow,public Person
{
    Q_OBJECT
public:
    explicit Lecturer_LogIn_Form(QWidget *parent = nullptr);
    ~Lecturer_LogIn_Form();
private:
    QString TEACHERID;
    QString PROFESSIONAL;
    Ui::Lecturer_LogIn_Form *ui;
    QTimer *timer;
    QSqlResult *result;
    bool checkTeacherId;
    bool checkProfessional;
    QToolButton* button;
private slots:
    void UpdateTime();
    void on_lineEdit_email_editingFinished();
    void on_lineEdit_cmnd_editingFinished();
    void on_lineEdit_phonenumber_editingFinished();
    void on_pushButton_clicked();
    void on_pushButton_exit_clicked();
    void keyReleaseEvent(QKeyEvent *event) override;
    void on_lineEdit_username_editingFinished();
    void on_lineEdit_address_editingFinished();
    void on_lineEdit_datebirth_editingFinished();
    void closeEvent(QCloseEvent *event) override;
    void on_lineEdit_teacherid_textChanged(const QString &arg1);
    void editingTeacherIdChange();
    bool checkTeacherIDExistInSystem();

public:
    bool checkDataIsNull() override;
    void getInformation() override;
    void insertIntoDatabase() override;
    bool checkForDuplicateData() override;
    void doNothing() override;
    void enterSignUpEvent();
    void enterExitEvent();
    void setCheckTeacherId(bool check){this->checkTeacherId = check;}
    void setCheckProfessional(bool check){this->checkProfessional = check;}
    void setProfessional(QString profess){this->PROFESSIONAL = profess;}
    void setTeacherId(QString id){this->TEACHERID = id;}
    QString getProfessional(){return this->PROFESSIONAL;}
    QString getTeacherId(){return this->TEACHERID;}
    bool getCheckTeacherId(){return this->checkTeacherId;}
    bool getCheckProfessonal(){return this->checkProfessional;}
    void initPasswordHint() override;
    void passwordShow() override;
    void passwordHint() override;
    void showButton();
    void hideButton();
};

#endif // LECTURER_LOGIN_FORM_H
