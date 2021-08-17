#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    teacherorstudent = new TeacherOrStudent();
    student_form_login = new Student_LogIn_Form();
    lecturer_form_login = new Lecturer_LogIn_Form();
    timer = new QTimer(this);
    ui->lineEdit_username->setFocus();
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(UpdateTime()));
    QObject::connect(ui->lineEdit_password,SIGNAL(returnPressed()),this,SLOT(enterClicked()));
    QObject::connect(ui->pushButton_signin,SIGNAL(returnPressed()),this,SLOT(enterClicked()));
    QObject::connect(ui->lineEdit_username,SIGNAL(returnPressed()),this,SLOT(nextLineEdit()));
    QObject::connect(ui->lineEdit_password,&QLineEdit::textChanged,this,&MainWindow::buttonShow);
    QObject::connect(ui->lineEdit_password,&QLineEdit::editingFinished,this,&MainWindow::buttonHide);
    timer->start(1000);
    initPasswordHint();
    readFile();
    pass= false;exitOut = true;
    qDebug()<<"DMMMMMMMMMMMMMMMMMMMMMMM";
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
    delete teacherorstudent;
    delete lecturer_form_login;
    delete student_form_login;
    delete student_Interface;
    delete teacher_Interface;
    delete button;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up)
    {
        this->focusPreviousChild();
    }
    if (event->key() == Qt::Key_Down)
    {
        this->focusNextChild();
    }
}

//take time current of system
void MainWindow::UpdateTime()
{
    ui->time_current->setText(QDate::currentDate().toString() +"  "+ QTime::currentTime().toString("hh:mm:ss"));
}



void MainWindow::on_pushButton_signup_clicked()
{
    teacherorstudent->close();
    teacherorstudent->show();
}

void MainWindow::on_pushButton_exit_clicked()
{
    emit(this->close());
}

void MainWindow::on_pushButton_signin_clicked()
{
    allCheck();
}

//check username and password are of teacher or student.
void MainWindow::allCheck()

{
    if (checkUsernamePasswordIsValid() == -2){
        messageBoxOneButton("Thông báo !","Vui lòng nhập tên tài khoản và mật khẩu !","Đồng ý",250,150,80,28);
    }

    if (checkUsernamePasswordIsValid() == 3)
    {
        setUsername(ui->lineEdit_username->text());
        setPassWord(ui->lineEdit_password->text());
        teacher_Interface = new Teacher_Interactive_Interface(nullptr,pass,getUsername(),getPassword());
        teacher_Interface->showMaximized();
        exitOut = false;
        this->close();
    }

    if (checkUsernamePasswordIsValid() == -3)
    {
        setUsername(ui->lineEdit_username->text());
        setPassWord(ui->lineEdit_password->text());
        student_Interface = new Student_Interactive_Interface(nullptr,pass,getUsername(),getPassword());
        student_Interface->showMaximized();
        exitOut = false;
        this->close();
    }

    if (checkUsernamePasswordIsValid() == 0)
    {
        messageBoxOneButton("Thông báo !","Tài khoản không tồn tại!","Đồng ý",250,150,80,28);
    }
}

//check username and password are existed and is of student or teacher
int MainWindow::checkUsernamePasswordIsValid()
{
    if (checkUsernamePasswordIsEmpty())
    {
        return -2;
    }

    if (checkUsernamePasswordTeacherIsTrue())
    {
        return 3;
    }

    if (checkUsernamePasswordStudentIsTrue())
    {
        return -3;
    }
    return 0;
}


//check username and password is of student
bool MainWindow::checkUsernamePasswordStudentIsTrue()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QSqlQuery queryStu;
    queryStu.prepare("SELECT USERNAME,PASSWORD FROM Student_DATABASE");
    if (queryStu.exec())
    {
        while (queryStu.next())
        {
            if (username == queryStu.value(0).toString() && password == queryStu.value(1).toString())
            {
                return true;
            }
        }
    }

    return false;
}

//check username and password is of teacher
bool MainWindow::checkUsernamePasswordTeacherIsTrue()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QSqlQuery queryTea;
    queryTea.prepare("SELECT USERNAME,PASSWORD FROM Lecturer_DATABASE");
    if (queryTea.exec())
    {
        while (queryTea.next())
        {
            if (username == queryTea.value(0).toString() && password == queryTea.value(1).toString())
            {
                return true;
            }
        }
    }

    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(exitOut){
        bool check = messageBoxTwoButton("Thông báo !","Bạn có muốn thoát chương trình không?\t","Đồng ý","Hủy",250,150,80,28);
        if(check){
            this->close();
            destroy(true,true);
            student_form_login->close();
            lecturer_form_login->close();
            teacherorstudent->close();
            qApp->exit();
        }
        else{
            event->ignore();
        }
    }
    else {
        this->close();
    }
}

//check username and password are empty
bool MainWindow::checkUsernamePasswordIsEmpty()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    if (username == NULL && password == NULL)
    {
        return true;
    }
    return false;
}


void MainWindow::enterClicked()
{
    allCheck();
}

void MainWindow::nextLineEdit()
{
    this->focusNextChild();
}

void MainWindow::changeStyleSheetWhenFocus()
{
    if (ui->lineEdit_username->hasFocus())
    {
    }
}

void MainWindow::showThis()
{
    this->show();
}

void MainWindow::buttonShow()
{
    button->show();
}

void MainWindow::buttonHide()
{
    button->hide();
}


void MainWindow::initPasswordHint()
{
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    QAction* action = ui->lineEdit_password->addAction(QIcon(":/icon-file/invisible.png"),QLineEdit::TrailingPosition);
    button = qobject_cast<QToolButton*>(action->associatedWidgets().last());
    button->hide();
    button->setCursor(QCursor(Qt::PointingHandCursor));
    connect(button,&QToolButton::pressed,this,&MainWindow::passwordShow);
    connect(button,&QToolButton::released,this,&MainWindow::passwordHint);
}

void MainWindow::passwordShow()
{
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    button->setIcon(QIcon(":/icon-file/password-Hint.png"));
    ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
}

void MainWindow::passwordHint()
{
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    button->setIcon(QIcon(":/icon-file/invisible.png"));
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}


void MainWindow::on_pushButton_clicked(){
    messageBoxOneButton("Thông báo !","Vui lòng liên hệ theo thông tin dưới đây để được hỗ trợ\t\t\n\n\t\tEmail : duykhannp7@gmail.com\n\t\tĐiện thoại : 0397439979","Đồng ý",600,200,80,28);
}


bool MainWindow::checkReturnTrue(QString ar)
{
    for (auto i = vecT.rbegin();i != vecT.rend() ; ++i) {
        if(i->getUser() == ar)
        {
            bool c = messageBoxTwoButton("Thông báo !","Tự động điền mật khẩu đã lưu !","Đồng ý","Hủy bỏ",350,150,85,28);
            if(c){ui->lineEdit_password->setText(i->getPass());pass=true;return true;}
            if(!c){pass = true;return true;}
        }
        pass = false;
    }
    return false;
}

void MainWindow::on_lineEdit_username_editingFinished()
{
    QString arg1 = ui->lineEdit_username->text();
    checkReturnTrue(arg1);
}
