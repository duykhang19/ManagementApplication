#include "ChangePassword.h"
#include "ui_ChangePassword.h"

ChangePassword::ChangePassword(QWidget *parent,QString username,QString password) :
    QWidget(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
    buttonA = new QToolButton();
    buttonB = new QToolButton();
    buttonC = new QToolButton();
    setUsername(username);
    setPassWord(password);
    initPasswordHint();
    qDebug()<<"ECCCCC ";readFile();

    QObject::connect(ui->lineEdit_oldpass,&QLineEdit::textChanged,this,&ChangePassword::buttonAShow);
    QObject::connect(ui->lineEdit_newpass,&QLineEdit::textChanged,this,&ChangePassword::buttonBShow);
    QObject::connect(ui->lineEdit_newpassres,&QLineEdit::textChanged,this,&ChangePassword::buttonCShow);

    QObject::connect(ui->lineEdit_oldpass,&QLineEdit::editingFinished,this,&ChangePassword::buttonAHide);
    QObject::connect(ui->lineEdit_newpass,&QLineEdit::editingFinished,this,&ChangePassword::buttonBHide);
    QObject::connect(ui->lineEdit_newpassres,&QLineEdit::editingFinished,this,&ChangePassword::buttonCHide);
}

ChangePassword::~ChangePassword()
{
    delete ui;
    delete buttonA;
    delete buttonB;
    delete buttonC;
}

void ChangePassword::initPasswordHint()//:/icon-file/boy_student.png
{
    ui->lineEdit_oldpass->setEchoMode(QLineEdit::Password);
    QAction* actionA = ui->lineEdit_oldpass->addAction(QIcon(":/icon-file/invisible.png"),QLineEdit::TrailingPosition);
    buttonA = qobject_cast<QToolButton*>(actionA->associatedWidgets().last());
    //buttonA->show();
    buttonA->hide();
    buttonA->setCursor(QCursor(Qt::PointingHandCursor));
    connect(buttonA,&QToolButton::pressed,this,&ChangePassword::passwordShow);
    connect(buttonA,&QToolButton::released,this,&ChangePassword::passwordHint);

    ui->lineEdit_newpass->setEchoMode(QLineEdit::Password);
    QAction* actionB = ui->lineEdit_newpass->addAction(QIcon(":/icon-file/invisible.png"),QLineEdit::TrailingPosition);
    buttonB = qobject_cast<QToolButton*>(actionB->associatedWidgets().last());
    //buttonB->show();
    buttonB->hide();
    buttonB->setCursor(QCursor(Qt::PointingHandCursor));
    connect(buttonB,&QToolButton::pressed,this,&ChangePassword::passwordShow);
    connect(buttonB,&QToolButton::released,this,&ChangePassword::passwordHint);

    ui->lineEdit_newpassres->setEchoMode(QLineEdit::Password);
    QAction* actionC = ui->lineEdit_newpassres->addAction(QIcon(":/icon-file/invisible.png"),QLineEdit::TrailingPosition);
    buttonC = qobject_cast<QToolButton*>(actionC->associatedWidgets().last());
    //buttonC->show();
    buttonC->hide();
    buttonC->setCursor(QCursor(Qt::PointingHandCursor));
    connect(buttonC,&QToolButton::pressed,this,&ChangePassword::passwordShow);
    connect(buttonC,&QToolButton::released,this,&ChangePassword::passwordHint);
}

void ChangePassword::passwordShow()
{
    if (ui->lineEdit_oldpass->hasFocus())
    {
        QToolButton* buttonA = qobject_cast<QToolButton*>(sender());
        ui->lineEdit_oldpass->setEchoMode(QLineEdit::Normal);
        buttonA->setIcon(QIcon(":/icon-file/password-Hint.png"));
    }

    if (ui->lineEdit_newpass->hasFocus())
    {
        QToolButton* buttonB = qobject_cast<QToolButton*>(sender());
        ui->lineEdit_newpass->setEchoMode(QLineEdit::Normal);
        buttonB->setIcon(QIcon(":/icon-file/password-Hint.png"));
    }

    if (ui->lineEdit_newpassres->hasFocus())
    {
        QToolButton* buttonC = qobject_cast<QToolButton*>(sender());
        ui->lineEdit_newpassres->setEchoMode(QLineEdit::Normal);
        buttonC->setIcon(QIcon(":/icon-file/password-Hint.png"));
    }
}

void ChangePassword::passwordHint()
{
    if (ui->lineEdit_oldpass->hasFocus())
    {
        QToolButton* buttonA = qobject_cast<QToolButton*>(sender());
        buttonA->setIcon(QIcon(":/icon-file/invisible.png"));
        ui->lineEdit_oldpass->setEchoMode(QLineEdit::Password);
    }

    if (ui->lineEdit_newpass->hasFocus())
    {
        QToolButton* buttonB = qobject_cast<QToolButton*>(sender());
        buttonB->setIcon(QIcon(":/icon-file/invisible.png"));
        ui->lineEdit_newpass->setEchoMode(QLineEdit::Password);
    }

    if (ui->lineEdit_newpassres->hasFocus())
    {
        QToolButton* buttonC = qobject_cast<QToolButton*>(sender());
        buttonC->setIcon(QIcon(":/icon-file/invisible.png"));
        ui->lineEdit_newpassres->setEchoMode(QLineEdit::Password);
    }
}

QString ChangePassword::getOldPassword(){return this->oldPass;}
QString ChangePassword::getNewPassword(){return this->newPass;}
QString ChangePassword::getNewResPassword(){return this->newPassRes;}
void ChangePassword::setOldPassword(QString a){this->oldPass = a;}
void ChangePassword::setNewPassword(QString a){this->newPass = a;};
void ChangePassword::setNewResPassword(QString a){this->newPassRes = a;};

bool ChangePassword::checkDataIsNulll()
{
    if (checkOldPasswordIsNull() || checkNewPasswordIsNull() || checkNewPasswordResIsNull()) {
        return true;
    }
    return false;
}

bool ChangePassword::checkOldPasswordIsSame()
{
    if (getPassword() == getOldPassword())
    {
        return true;
    }
    return false;
}
bool ChangePassword::checkNewPasswordIsSame()
{
    if (getNewPassword() == getNewResPassword()) {
        return true;
    }
    return false;
}
bool ChangePassword::checkOldPasswordIsNull()
{
    if (getOldPassword().isEmpty()) {
        return true;
    }
    return false;
}
bool ChangePassword::checkNewPasswordIsNull()
{
    if (getNewPassword().isEmpty()) {
        return true;
    }
    return false;
}
bool ChangePassword::checkNewPasswordResIsNull()
{
    if (getNewResPassword().isEmpty()) {
        return true;
    }
    return false;
}



void ChangePassword::doIt()
{
    AutoFillPassWord k;
    setOldPassword(ui->lineEdit_oldpass->text());
    setNewPassword(ui->lineEdit_newpass->text());
    setNewResPassword(ui->lineEdit_newpassres->text());
    if (!checkDataIsNulll() && checkNewPasswordIsSame() && checkOldPasswordIsSame()){
        QSqlQuery qury;
        if(getNum() == 1){qury.prepare("UPDATE Student_DATABASE SET PASSWORD=:newpass WHERE USERNAME=:username");}
        if(getNum() == 2){qury.prepare("UPDATE Lecturer_DATABASE SET PASSWORD=:newpass WHERE USERNAME=:username");}
        qury.bindValue(0,getNewPassword());
        qury.bindValue(1,getUsername());
        //qury.exec();
        if (qury.exec()) {
            qDebug()<<"User :"<<getUsername()<<" pass:"<<getNewPassword();
            setSaveOrNot(true);k.setBool(true);k.setUser(getUsername());k.setPass(getNewPassword());vecT.push_back(k);writeToFile();
            messageBoxOneButton("Thông báo","\t  Đổi mật khẩu thành công !\t\t","Đồng ý",450,200,80,25);this->close();
        }
        else{messageBoxOneButton("Thông báo","\tĐổi mật khẩu không thành công !\t\t","Đồng ý",450,200,80,25);}
    }
    if (checkDataIsNulll() || !checkNewPasswordIsSame() || !checkOldPasswordIsSame()) {
        messageBoxOneButton("Thông báo","\tĐổi mật khẩu không thành công !\t\t","Đồng ý",450,200,80,25);
    }
}

void ChangePassword::setNum(int c){this->a = c;}

void ChangePassword::buttonAShow()
{
    buttonA->show();
}

void ChangePassword::buttonBShow()
{
    buttonB->show();
}

void ChangePassword::buttonCShow()
{
    buttonC->show();
}

void ChangePassword::buttonAHide()
{
    buttonA->hide();
}
void ChangePassword::buttonBHide()
{
    buttonB->hide();
}
void ChangePassword::buttonCHide()
{
    buttonC->hide();
}

int ChangePassword::getNum(){return this->a;}

void ChangePassword::on_pushButton_clicked()
{
    doIt();
}



void ChangePassword::on_lineEdit_oldpass_textChanged(const QString &arg1)
{
    setOldPassword(arg1);
    if (getOldPassword().isEmpty() || !checkOldPasswordIsSame()){
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_oldpass->mapToGlobal(QPoint()),tr("Mật khẩu cũ không khớp nhau!"));
        QToolTip::setPalette(pal);
        ui->lineEdit_oldpass->setStyleSheet("color : red");
    }
    if(!getOldPassword().isEmpty() && checkOldPasswordIsSame())
    {
        ui->lineEdit_oldpass->setStyleSheet("color:black");
        QToolTip::hideText();
    }
}

void ChangePassword::on_lineEdit_newpassres_textChanged(const QString &arg1)
{
    setNewResPassword(arg1);
    if (!checkNewPasswordIsSame()) {
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_newpassres->mapToGlobal(QPoint()),tr("Mật khẩu mới không khớp nhau!"));
        QToolTip::setPalette(pal);
        ui->lineEdit_newpassres->setStyleSheet("color : red");
    }else{ui->lineEdit_newpassres->setStyleSheet("color:black");QToolTip::hideText();}
}

void ChangePassword::on_lineEdit_newpass_textChanged(const QString &arg1)
{
    setNewPassword(arg1);
}
