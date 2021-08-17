#include "Lecturer_LogIn_Form.h"
#include "ui_Lecturer_LogIn_Form.h"
#include<QDate>
#include<QTimer>
#include<QTime>
#include<QDateTime>
#include<QMessageBox>
#include<QMouseEvent>
#include<QEvent>
#include<QDebug>
#include<QString>
#include<QSqlQuery>
#include<QCalendar>
#include<QCalendarWidget>
#include<QSqlResult>
#include<sstream>
#include<algorithm>
#include<vector>
#include<iostream>
#include<QList>

Lecturer_LogIn_Form::Lecturer_LogIn_Form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Lecturer_LogIn_Form)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(UpdateTime()));
    QObject::connect(ui->lineEdit_teacherid,&QLineEdit::textChanged,this,&Lecturer_LogIn_Form::editingTeacherIdChange);
    QObject::connect(ui->lineEdit_password,&QLineEdit::textChanged,this,&Lecturer_LogIn_Form::showButton);
    QObject::connect(ui->lineEdit_password,&QLineEdit::editingFinished,this,&Lecturer_LogIn_Form::hideButton);
    timer->start(1000);
    ui->lineEdit_cmnd->setMaxLength(9);
    ui->lineEdit_phonenumber->setMaxLength(10);
    ui->lineEdit_teacherid->setMaxLength(10);
    ui->lineEdit_name->setFocus();
    ui->lineEdit_professional->setCursorPosition(0);
    ui->lineEdit_name->setStyleSheet("color : black");
    ui->lineEdit_professional->setStyleSheet("color : black");
    initPasswordHint();

}

Lecturer_LogIn_Form::~Lecturer_LogIn_Form()
{
    delete ui;
    delete button;
    delete timer;
    delete result;
    delete button;
}

// get information of teacher and store to database SQL Server
void Lecturer_LogIn_Form::enterSignUpEvent()
{
    getInformation();
}

//close current widget when get close event.
void Lecturer_LogIn_Form::enterExitEvent()
{
    close();
}

//function to take time of system.
void Lecturer_LogIn_Form::UpdateTime()
{
    ui->time_current->setText(QDate::currentDate().toString() +"  "+ QTime::currentTime().toString("hh:mm:ss"));
}


//function get information of teacher
void Lecturer_LogIn_Form::getInformation()
{
    if (checkDataIsNull())
    {
        messageBoxOneButton("Thông báo !","Vui lòng điền đầy đủ thông tin !","Đồng ý",250,150,80,28);
    }
    if(!checkDataIsNull())
    {
        this->setTeacherId(ui->lineEdit_teacherid->text());
        this->setName(ui->lineEdit_name->text());
        this->setAddress(ui->lineEdit_address->text());
        this->setUsername(ui->lineEdit_username->text());
        this->setPassWord(ui->lineEdit_password->text());
        this->setPhonenumber(ui->lineEdit_phonenumber->text());
        if (ui->radioButton_male->isChecked())
        {
            this->setSex("");
            this->setSex("Nam");
        }
        if(ui->radioButton_female->isChecked())
        {
            this->setSex("");
            this->setSex("Nữ");
        }
        this->setGmail(ui->lineEdit_email->text());
        this->setProfessional(ui->lineEdit_professional->text());
        this->setIdentitycard(ui->lineEdit_cmnd->text());
        this->setDatebirth(ui->lineEdit_datebirth->text());
        //QString mssv,QString cmnd,QString gmail,QString username,QString phonenumber
        if (!checkForDuplicateData())
        {
            ui->lineEdit_email->setStyleSheet("color :black");ui->lineEdit_phonenumber->setStyleSheet("color : black");ui->lineEdit_cmnd->setStyleSheet("color : black");ui->lineEdit_teacherid->setStyleSheet("color : black");ui->lineEdit_username->setStyleSheet("color : black");
            insertIntoDatabase();
        }
        else
        {
            doNothing();
        }
    }
}

void Lecturer_LogIn_Form::doNothing()
{}

//function to insert data to database.
void Lecturer_LogIn_Form::insertIntoDatabase()
{
    qDebug()<<getCheckTeacherId()<<" "<<getCheckGmail()<<" "<<getCheckPhonenumber()<<" "<<getCheckIdentitycard()<<" "<<getCheckAddress()<<" "<<getCheckDatebirth()<<" "<<getCheckUsername()<<" "<<checkTeacherIDExistInSystem();
    if (getCheckTeacherId() && getCheckGmail() && getCheckPhonenumber() && getCheckIdentitycard() && getCheckAddress() && getCheckDatebirth() && getCheckUsername() &&  checkTeacherIDExistInSystem())
    {
        QSqlQuery query,qury;
        QString date_birth = getDatebirth();
        QDate date = QDate::fromString(date_birth,"dd-MM-yyyy");
        date_birth = date.toString("yyyy-MM-dd");
        qury.prepare("INSERT INTO IMAGE_USERS(userID) VALUES(:id)");
        qury.bindValue(0,getTeacherId());
        query.prepare("INSERT INTO Lecturer_DATABASE(MSGV,NAME,PROFESSIONAL,CMND,PHONENUMBER,GMAIL,ADDRESS,SEX,DATEBIRTH,USERNAME,PASSWORD) VALUES(:MSGV,:NAME,:PROFESSIONAL,:CMND,:PHONENUMBER,:GMAIL,:ADDRESS,:SEX,CAST(:DATEBIRTH AS DATE),:USERNAME,:PASSWORD)");
        query.bindValue(0,getTeacherId());
        query.bindValue(1,getName());
        query.bindValue(2,getProfessional());
        query.bindValue(3,getIdentitycard());
        query.bindValue(4,getPhonenumber());
        query.bindValue(5,getGmail());
        query.bindValue(6,getAddress());
        query.bindValue(7,getSex());
        query.bindValue(8,date_birth);
        query.bindValue(9,getUsername());
        query.bindValue(10,getPassword());
        if (query.exec() && qury.exec())
        {
            messageBoxOneButton("Thông báo !","Tạo tài khoản thành công !","Đồng ý",300,150,80,28);
        }
    }
    else
    {
        messageBoxOneButton("Thông báo !","Tạo tài khoản thất bại !","Đồng ý",250,150,80,28);
    }
};


//function to check data null or not.
bool Lecturer_LogIn_Form::checkDataIsNull()
{
    if (ui->lineEdit_teacherid->text().isEmpty() || ui->lineEdit_datebirth->text().isEmpty() || ui->lineEdit_name->text().isEmpty() || ui->lineEdit_username->text().isEmpty() || ui->lineEdit_password->text().isEmpty() || ui->lineEdit_cmnd->text().isEmpty() || ui->lineEdit_email->text().isEmpty() || ui->lineEdit_phonenumber->text().isEmpty() || ui->lineEdit_professional->text().isEmpty() || ui->lineEdit_address->text().isEmpty())
    {
        return true;
    }
    return false;
}


//get informaton of teacher when get clicked()
void Lecturer_LogIn_Form::on_pushButton_clicked()
{
    getInformation();
}


//check data has been duplicated or not
bool Lecturer_LogIn_Form::checkForDuplicateData()
{
    QString _teacherid = "";
    QString _cmnd ="";
    QString _gmail = "";
    QString _username = "";
    QString _phonenumber = "";
    QString resultStr ="";
    QSqlQuery sqlquery,querystu,qury;
    int i =0;bool nl = false;
    sqlquery.prepare("SELECT MSGV,CMND,GMAIL,USERNAME,PHONENUMBER FROM Lecturer_DATABASE");
    querystu.prepare("SELECT MSSV,CMND,GMAIL,USERNAME,PHONENUMBER FROM Student_DATABASE");
    qury.prepare("SELECT MSGV,CMND,GMAIL,USERNAME,PHONENUMBER FROM Lecturer_DATABASE WHERE MSGV ='"+getTeacherId()+"'");
    qury.exec();
    qury.first();
    if(qury.value(0).toString() == getTeacherId() && qury.value(1).toString() == getIdentitycard() && qury.value(2).toString() == getGmail() && qury.value(3).toString() == getUsername() && qury.value(4).toString() == getPhonenumber()){nl = true;messageBoxOneButton("Thông báo !","Tài khoản này đã tồn tại !","Đồng ý",250,150,85,28);return true;}

    if(sqlquery.exec() && querystu.exec() && !nl)
    {
        qDebug()<<"done 1";
        while (sqlquery.next() || querystu.next())
        {
            if(getTeacherId() == sqlquery.value(0).toString() || getTeacherId() == querystu.value(0).toString()){_teacherid = "\t\tMã giảng viên\n";ui->lineEdit_teacherid->setStyleSheet("color : red");};
            if (getIdentitycard() == sqlquery.value(1).toString() || getIdentitycard() == querystu.value(1).toString()){_cmnd = "\t\tCăn cước công dân\n";ui->lineEdit_cmnd->setStyleSheet("color : red");};
            if (getGmail() == sqlquery.value(2).toString() || getGmail() == querystu.value(2).toString()){_gmail = "\t\tEmail\n";ui->lineEdit_email->setStyleSheet("color : red");};
            if (getUsername() == sqlquery.value(3).toString() || getUsername() == querystu.value(3).toString()){_username = "\t\tTên tài khoản\n";ui->lineEdit_username->setStyleSheet("color : red");};
            if (getPhonenumber() == sqlquery.value(4).toString() || getPhonenumber() == querystu.value(4).toString()){_phonenumber = "\t\tSố điện thoại\n";ui->lineEdit_phonenumber->setStyleSheet("color : red");};
        }
    }
    resultStr =_teacherid +  _gmail + _phonenumber + _cmnd + _username;
    if(!resultStr.isEmpty() && !nl)
    {
        messageBoxOneButton("Thông báo !",resultStr+"những thông tin trên của bạn đã được sử dụng !","Đồng ý",350,200,90,28);
        //ui->lineEdit_email->setStyleSheet("color : red");ui->lineEdit_phonenumber->setStyleSheet("color : red");ui->lineEdit_cmnd->setStyleSheet("color : red");ui->lineEdit_teacherid->setStyleSheet("color : red");ui->lineEdit_username->setStyleSheet("color : red");
        resultStr.clear();
        return true;
    }
    resultStr.clear();
    return false;
};



//check email is valid or not
void Lecturer_LogIn_Form::on_lineEdit_email_editingFinished()
{
    QRegularExpression rec("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",QRegularExpression::CaseInsensitiveOption);
    QString emaill = ui->lineEdit_email->text();
    QRegularExpressionMatch rec_math = rec.match(emaill);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QSqlQuery querylec,querystu;
        querylec.prepare("SELECT GMAIL FROM Lecturer_DATABASE WHERE GMAIL = '"+ui->lineEdit_email->text()+"'");
        querystu.prepare("SELECT GMAIL FROM Student_DATABASE WHERE GMAIL = '"+ui->lineEdit_email->text()+"'");
        querystu.exec();
        querylec.exec();
        querylec.first();
        querystu.first();
        if (querylec.value(0).toString() == ui->lineEdit_email->text() || querystu.value(0).toString() == ui->lineEdit_email->text())
        {
           QToolTip::hideText();
           QPalette pal;
           pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
           QToolTip::showText(ui->lineEdit_email->mapToGlobal(QPoint()),tr("Email đã được sử dụng !"));
           QToolTip::setPalette(pal);
           ui->lineEdit_email->setStyleSheet("color : red");
           //this->checkGmail = false;
           this->setCheckGmail(false);
        }
        else
        {
            QToolTip::hideText();
            ui->lineEdit_email->setStyleSheet("color : black");
            //this->checkGmail = true;
            this->setCheckGmail(true);
        }
    }
    else
    {
        QToolTip::hideText();
        ui->lineEdit_email->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_email->mapToGlobal(QPoint()), tr("Email không hợp lệ !"));
        QToolTip::setPalette(pal);
        //this->checkGmail = false;
        this->setCheckGmail(false);
    }
}

//check identity card is valid or not;
void Lecturer_LogIn_Form::on_lineEdit_cmnd_editingFinished()
{
    QRegularExpression rec("([0-9]{9})");
    QString cmnd = ui->lineEdit_cmnd->text();
    QRegularExpressionMatch rec_match = rec.match(cmnd);
    bool math_return = rec_match.hasMatch();
    if (math_return)
    {
        QSqlQuery querystu,querylex;
        querystu.prepare("SELECT CMND FROM Lecturer_DATABASE WHERE CMND = '"+ui->lineEdit_cmnd->text()+"'");
        querylex.prepare("SELECT CMND FROM Student_DATABASE WHERE CMND = '"+ui->lineEdit_cmnd->text()+"'");
        querystu.exec();
        querylex.exec();
        querystu.first();
        querylex.first();
        if (querystu.value(0).toString() == ui->lineEdit_cmnd->text() || querylex.value(0).toString() == ui->lineEdit_cmnd->text())
        {
           QToolTip::hideText();
           QPalette pal;
           pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
           QToolTip::showText(ui->lineEdit_cmnd->mapToGlobal(QPoint()),tr("Căn cước đã được sử dụng !"));
           QToolTip::setPalette(pal);
           ui->lineEdit_cmnd->setStyleSheet("color : red");
           this->setCheckIdentitycard(false);
        }
        else
        {
            QToolTip::hideText();
            ui->lineEdit_cmnd->setStyleSheet("color : black");
            this->setCheckIdentitycard(true);
        }
    }
    else
    {
        QToolTip::hideText();
        ui->lineEdit_cmnd->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_cmnd->mapToGlobal(QPoint()), tr("Căn cước không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->setCheckIdentitycard(false);
    }
}

//check phone number is valid or not.
void Lecturer_LogIn_Form::on_lineEdit_phonenumber_editingFinished()
{
    QRegularExpression rec("^[0]{1}[0-9]{9}");
    QString phone_number = ui->lineEdit_phonenumber->text();
    QRegularExpressionMatch rec_math = rec.match(phone_number);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QSqlQuery querylec,querystu;
        querylec.prepare("SELECT PHONENUMBER FROM Lecturer_DATABASE WHERE PHONENUMBER = '"+ui->lineEdit_phonenumber->text()+"'");
        querystu.prepare("SELECT PHONENUMBER FROM Student_DATABASE WHERE PHONENUMBER = '"+ui->lineEdit_phonenumber->text()+"'");
        querylec.exec();
        querystu.exec();
        querylec.first();
        querystu.first();
        if (querylec.value(0).toString() == ui->lineEdit_phonenumber->text() || querystu.value(0).toString() == ui->lineEdit_phonenumber->text())
        {
           QToolTip::hideText();
           QPalette pal;
           pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
           QToolTip::showText(ui->lineEdit_phonenumber->mapToGlobal(QPoint()),tr("Số điện thoại đã được sử dụng !"));
           QToolTip::setPalette(pal);
           ui->lineEdit_phonenumber->setStyleSheet("color : red");
           this->setCheckPhonenumber(false);
        }
        else
        {
            QToolTip::hideText();
            ui->lineEdit_phonenumber->setStyleSheet("color : black");
            this->setCheckPhonenumber(true);
        }
    }
    else
    {
        QToolTip::hideText();
        ui->lineEdit_phonenumber->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_phonenumber->mapToGlobal(QPoint()), tr("Số điện thoại không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->setCheckPhonenumber(false);
    }
}


void Lecturer_LogIn_Form::on_pushButton_exit_clicked()
{
    close();
}

//get event of every key
void Lecturer_LogIn_Form::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down)
    {
        this->focusNextChild();
    }
    if (event->key() == Qt::Key_Up)
    {
        this->focusPreviousChild();
    }
}

void Lecturer_LogIn_Form::closeEvent(QCloseEvent *event)
{
    ui->lineEdit_name->clear();
    ui->lineEdit_teacherid->clear();
    ui->lineEdit_address->clear();
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_phonenumber->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_professional->clear();
    ui->lineEdit_datebirth->clear();
    ui->lineEdit_cmnd->clear();
    ui->lineEdit_name->setStyleSheet("color : black");
    ui->lineEdit_teacherid->setStyleSheet("color : black");
    ui->lineEdit_address->setStyleSheet("color : black");
    ui->lineEdit_username->setStyleSheet("color : black");
    ui->lineEdit_password->setStyleSheet("color : black");
    ui->lineEdit_phonenumber->setStyleSheet("color : black");
    ui->lineEdit_email->setStyleSheet("color : black");
    ui->lineEdit_professional->setStyleSheet("color : black");
    ui->lineEdit_datebirth->setStyleSheet("color : black");
    ui->lineEdit_cmnd->setStyleSheet("color : black");
    event->accept();
}
//check username is valid or not
void Lecturer_LogIn_Form::on_lineEdit_username_editingFinished()
{
    QSqlQuery query,querystu;
    query.prepare("SELECT USERNAME FROM Lecturer_DATABASE WHERE USERNAME = '"+ui->lineEdit_username->text()+"'");
    querystu.prepare("SELECT USERNAME FROM Student_DATABASE WHERE USERNAME = '"+ui->lineEdit_username->text()+"'");
    query.exec();
    query.first();
    querystu.exec();
    querystu.first();
    if (ui->lineEdit_username->text().isEmpty())
    {
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_username->mapToGlobal(QPoint()),tr("Tên tài khoản không hợp lệ !"));
        QToolTip::setPalette(pal);
        ui->lineEdit_username->setStyleSheet("color : red");
        this->setCheckUsername(false);
    }
    else if (query.value(0).toString() == ui->lineEdit_username->text() || querystu.value(0).toString() == ui->lineEdit_username->text())
    {
       QToolTip::hideText();
       QPalette pal;
       pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
       QToolTip::showText(ui->lineEdit_username->mapToGlobal(QPoint()),tr("Tên tài khoản đã được sử dụng !"));
       QToolTip::setPalette(pal);
       ui->lineEdit_username->setStyleSheet("color : red");
       this->setCheckUsername(false);
    }
    else
    {
        QToolTip::hideText();
        ui->lineEdit_username->setStyleSheet("color : black");
        this->setCheckUsername(true);
    }
}


//check address valid or not
void Lecturer_LogIn_Form::on_lineEdit_address_editingFinished()
{
    QRegularExpression rec("[a-zA-Z]",QRegularExpression::CaseInsensitiveOption);
    QString place = ui->lineEdit_address->text();
    QRegularExpressionMatch rec_math = rec.match(place);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QToolTip::hideText();
        ui->lineEdit_address->setStyleSheet("color : black");
        stringstream ss;
        string name="",temp="",str;
        QString name_ = ui->lineEdit_address->text();
        name_ = name_.toLower();
        ss<<name_.toStdString();
        while (!ss.eof())
        {
            ss>>temp;
            if (isalpha(temp[0]) && islower(temp[0]))
            {
                temp[0] = temp[0]-32;
            }
            name = name + temp +" ";
        }
        ui->lineEdit_address->clear();
        name.erase(name.size()-1);
        ui->lineEdit_address->setText(name.c_str());
        this->setCheckAddress(true);
    }
    if(!it)
    {
        QToolTip::hideText();
        ui->lineEdit_address->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_address->mapToGlobal(QPoint()), tr("Địa chỉ không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->setCheckAddress(false);
    }
}

void Lecturer_LogIn_Form::on_lineEdit_datebirth_editingFinished()
{
    string dater= ui->lineEdit_datebirth->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_datebirth->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_datebirth->text(),"dd-MM-yyyy");
    if (!date.isValid())
    {
        QToolTip::hideText();
        ui->lineEdit_datebirth->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_datebirth->mapToGlobal(QPoint()),tr("Ngày sinh không hợp lệ !"));
        QToolTip::setPalette(pal);
        //this->checkDatebirth = false;
        this->setCheckDatebirth(false);
    }
    if (date.isValid())
    {
        ui->lineEdit_datebirth->setStyleSheet("color : black");
        this->setCheckDatebirth(true);
    }
}

void Lecturer_LogIn_Form::initPasswordHint()
{
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    QAction* action = ui->lineEdit_password->addAction(QIcon(":/icon-file/invisible.png"),QLineEdit::TrailingPosition);
    button = qobject_cast<QToolButton*>(action->associatedWidgets().last());
    button->hide();
    button->setCursor(QCursor(Qt::PointingHandCursor));
    connect(button,&QToolButton::pressed,this,&Lecturer_LogIn_Form::passwordShow);
    connect(button,&QToolButton::released,this,&Lecturer_LogIn_Form::passwordHint);
}

void Lecturer_LogIn_Form::passwordShow()
{
    QToolButton* button = qobject_cast<QToolButton*>(sender());
    ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    button->setIcon(QIcon(":/icon-file/password-Hint.png"));
}

void Lecturer_LogIn_Form::passwordHint()
{
    QToolButton* button = qobject_cast<QToolButton*>(sender());
    button->setIcon(QIcon(":/icon-file/invisible.png"));
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

void Lecturer_LogIn_Form::showButton()
{
    button->show();
}

void Lecturer_LogIn_Form::hideButton()
{
    button->hide();
}

void Lecturer_LogIn_Form::on_lineEdit_teacherid_textChanged(const QString &arg1)
{
    QRegularExpression rec("([a-zA-Z0-9])",QRegularExpression::CaseInsensitiveOption);
    QString mssv = ui->lineEdit_teacherid->text();
    QRegularExpressionMatch rec_match = rec.match(mssv);
    bool match_rec = rec_match.hasMatch();

    if (match_rec)
    {
        QSqlQuery querystu,querylec;
        querystu.prepare("SELECT MSSV FROM Student_DATABASE WHERE MSSV = '"+ui->lineEdit_teacherid->text()+"'");
        querylec.prepare("SELECT MSGV FROM Lecturer_DATABASE WHERE MSGV = '"+ui->lineEdit_teacherid->text()+"'");
        querystu.exec();
        querylec.exec();
        querystu.first();
        querylec.first();
        if (querystu.value(0).toString() == ui->lineEdit_teacherid->text() || querylec.value(0).toString() == ui->lineEdit_teacherid->text())
        {
           QToolTip::hideText();
           QPalette pal;
           pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
           QToolTip::showText(ui->lineEdit_teacherid->mapToGlobal(QPoint()),tr("Mã giáo viên đã được sử dụng !"));
           QToolTip::setPalette(pal);
           ui->lineEdit_teacherid->setStyleSheet("color : red");
           this->setCheckTeacherId(false);
        }
        else
        {
            if (!checkTeacherIDExistInSystem()){
                QToolTip::hideText();
                QPalette pal;
                pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
                QToolTip::setPalette(pal);
                ui->lineEdit_teacherid->setStyleSheet("color : red");
                QToolTip::showText(ui->lineEdit_teacherid->mapToGlobal(QPoint()),tr("Bạn chưa được ghi danh trong hệ thống giáo viên của trường !"));
            }
            else
            {
                QToolTip::hideText();
                ui->lineEdit_teacherid->setStyleSheet("color : black");
                this->setCheckTeacherId(true);
                string id = ui->lineEdit_teacherid->text().toStdString();
                for (unsigned long long i =0 ;i<id.size() ;++i ) {
                    if (isalpha(id[i]) && islower(id[i])) {
                        id[i]=id[i]-32;
                    }
                }
                ui->lineEdit_teacherid->setText(id.c_str());
            }
        }
    }
    else
    {
        ui->lineEdit_teacherid->setStyleSheet("color : red");
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_teacherid->mapToGlobal(QPoint()),tr("Mã giáo viên không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->setCheckTeacherId(false);
    }
}

void Lecturer_LogIn_Form::editingTeacherIdChange()
{
    QString teacherid = ui->lineEdit_teacherid->text();
    QSqlQuery qury;
    qury.prepare("SELECT Professional,nameTeacher FROM Teacher WHERE MSGV=:teacherid");
    qury.bindValue(0,teacherid);
    qury.exec();
    qury.first();
    ui->lineEdit_professional->setText(qury.value(0).toString());
    ui->lineEdit_name->setText(qury.value(1).toString());
}


bool Lecturer_LogIn_Form::checkTeacherIDExistInSystem()
{
    QSqlQuery qury;
    QString id = ui->lineEdit_teacherid->text();
    qury.prepare("SELECT MSGV FROM Teacher WHERE MSGV='"+id+"'");
    qury.exec();
    qury.first();
    if(qury.value(0).toString() == id){return true;}
    return false;
}

