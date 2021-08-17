#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include<QDate>
#include<QString>
#include"TeacherOrStudent.h"
#include<QSqlQuery>
#include<QDebug>
#include"Lecturer_LogIn_Form.h"
#include"Student_LogIn_Form.h"
#include"Student_Interactive_Interface.h"
#include<QMessageBox>
#include<QSizePolicy>
#include<QEnterEvent>
#include<QFocusEvent>
#include<QLineEdit>
#include"Teacher_Interactive_Interface.h"
#include<QCloseEvent>
#include<QKeyEvent>
#include<QAbstractButton>
#include<QCursor>
#include"Person.h"
#include"ChangePassword.h"
#include<QMouseEvent>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow,public Person
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void showThis();
    void buttonShow();
    void buttonHide();
    void initPasswordHint() override;
    void passwordShow() override;
    void passwordHint() override;
    ~MainWindow();
private slots:
    void UpdateTime();
    void on_pushButton_signup_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_signin_clicked();
    bool checkUsernamePasswordIsEmpty();
    bool checkUsernamePasswordStudentIsTrue();
    bool checkUsernamePasswordTeacherIsTrue();
    int checkUsernamePasswordIsValid();
    void closeEvent(QCloseEvent *event) override;
    void allCheck();
    void changeStyleSheetWhenFocus();
    void nextLineEdit();
    void enterClicked();
    void keyReleaseEvent(QKeyEvent *event) override;     
    void on_pushButton_clicked();

    bool checkReturnTrue(QString);
    void on_lineEdit_username_editingFinished();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    TeacherOrStudent *teacherorstudent;
    Student_LogIn_Form *student_form_login;
    Lecturer_LogIn_Form *lecturer_form_login;
    Student_Interactive_Interface *student_Interface;
    Teacher_Interactive_Interface *teacher_Interface;
    QToolButton* button;
    bool pass,exitOut;
};

#endif // MAINWINDOW_H
