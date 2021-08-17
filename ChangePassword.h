#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QWidget>
#include<QAction>
#include<QCursor>
#include"Person.h"
#include<QToolButton>
#include<QLineEdit>
#include<QSqlQuery>
#include<QToolTip>
#include"AutoFillPassWord.h"
#include<vector>

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QWidget, public Person
{
    Q_OBJECT

public:
    explicit ChangePassword(QWidget *parent = nullptr,QString username=NULL,QString password = NULL);
    void initPasswordHint() override;
    void passwordShow() override;
    void passwordHint() override;
    QString getOldPassword();
    QString getNewPassword();
    QString getNewResPassword();
    void setOldPassword(QString);
    void setNewPassword(QString);
    void setNewResPassword(QString);
    bool checkOldPasswordIsSame();
    bool checkNewPasswordIsSame();
    bool checkOldPasswordIsNull();
    bool checkNewPasswordIsNull();
    bool checkNewPasswordResIsNull();
    void doIt();
    void setNum(int);
    void buttonAShow();
    void buttonBShow();
    void buttonCShow();
    void buttonAHide();
    void buttonBHide();
    void buttonCHide();
    int getNum();
    bool checkDataIsNulll() ;
    ~ChangePassword();

private slots:
    void on_pushButton_clicked();
    void on_lineEdit_oldpass_textChanged(const QString &arg1);
    void on_lineEdit_newpassres_textChanged(const QString &arg1);
    void on_lineEdit_newpass_textChanged(const QString &arg1);

private:
    Ui::ChangePassword *ui;
    QString oldPass;
    QString newPass;
    QString newPassRes;
    QToolButton* buttonA;
    QToolButton* buttonB;
    QToolButton* buttonC;
    int a;
};

#endif // CHANGEPASSWORD_H
