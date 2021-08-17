#include "Student_LogIn_Form.h"
#include "ui_Student_LogIn_Form.h"
#include<QDate>
#include<QTime>
#include<QMessageBox>
#include<QSqlQuery>
#include<sstream>
#include<cctype>


Student_LogIn_Form::Student_LogIn_Form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Student_LogIn_Form)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(UpdateTime()));
    QObject::connect(ui->lineEdit_mssvs,&QLineEdit::textChanged,this,&Student_LogIn_Form::editingStudentIDFinished);
    QObject::connect(ui->lineEdit_passwords,&QLineEdit::textChanged,this,&Student_LogIn_Form::showButton);
    QObject::connect(ui->lineEdit_passwords,&QLineEdit::editingFinished,this,&Student_LogIn_Form::hideButton);
    timer->start(1000);
    ui->lineEdit_phonenumbers->setMaxLength(10);
    ui->lineEdit_cmnds->setMaxLength(9);
    ui->lineEdit_mssvs->setMaxLength(10);
    ui->lineEdit_names->setFocus();
    ui->lineEdit_names->setStyleSheet("color : black");
    ui->lineEdit_classnames->setStyleSheet("color : black");
    ui->lineEdit_majors->setStyleSheet("color : black");
    initPasswordHint();
}

Student_LogIn_Form::~Student_LogIn_Form()
{
    delete ui;
    delete timer;
    delete button;
}

void Student_LogIn_Form::UpdateTime()
{
    ui->time_currents->setText(QDate::currentDate().toString() +"  "+QTime::currentTime().toString("hh:mm:ss"));
}


void Student_LogIn_Form::enterEventExit()
{
    close();
}

//check username,password and insert into database.
void Student_LogIn_Form::getInformation()
{
    if(checkDataIsNull())
    {
        messageBoxOneButton("Thông báo !","Vui lòng điền đầy đủ thông tin !","Đồng ý",450,150,80,28);
    }
    else
    {
        this->setName(ui->lineEdit_names->text());
        this->setStudentId(ui->lineEdit_mssvs->text());
        this->setAddress(ui->lineEdit_addresss->text());
        this->setPhonenumber(ui->lineEdit_phonenumbers->text());
        this->setGmail(ui->lineEdit_emails->text());
        this->setClassname(ui->lineEdit_classnames->text());
        this->setMajor(ui->lineEdit_majors->text());
        this->setDatebirth(ui->lineEdit_datebirths->text());
        this->setUsername(ui->lineEdit_usernames->text());
        this->setPassWord(ui->lineEdit_passwords->text());
        this->setIdentitycard(ui->lineEdit_cmnds->text());
        if(ui->radioButton_male->isChecked())
        {
            this->setSex("Nam");
        }
        if (ui->radioButton_female->isChecked())
        {
            this->setSex("Nữ");
        }

        if (!checkForDuplicateData())
        {
            ui->lineEdit_cmnds->setStyleSheet("color:black");ui->lineEdit_mssvs->setStyleSheet("color:black");ui->lineEdit_emails->setStyleSheet("color:black");ui->lineEdit_usernames->setStyleSheet("color:black");ui->lineEdit_phonenumbers->setStyleSheet("color:black");
            insertIntoDatabase();
        }
        else
        {
            doNothing();
        }
     }
}

//check data is empty.
bool Student_LogIn_Form::checkDataIsNull()
{
    if (ui->lineEdit_names->text().isEmpty() || ui->lineEdit_classnames->text().isEmpty() || ui->lineEdit_mssvs->text().isEmpty() || ui->lineEdit_usernames->text().isEmpty() || ui->lineEdit_passwords->text().isEmpty() || ui->lineEdit_cmnds->text().isEmpty() || ui->lineEdit_emails->text().isEmpty() || ui->lineEdit_phonenumbers->text().isEmpty() || ui->lineEdit_majors->text().isEmpty() || ui->lineEdit_addresss->text().isEmpty())
    {
        return true;
    }
    return false;
}


void Student_LogIn_Form::insertIntoDatabase()
{
    qDebug()<<"INSERT : "<<getCheckName()<<" "<<getCheckUsername()<<" "<<getCheckStudentId()<<" "<<getCheckAddress()<<" "<<getCheckPhonenumber()<<" "<<getCheckGmail()<<" "<<getCheckDatebirth()<<" "<<getCheckIdentitycard();
    if (getCheckUsername() && getCheckStudentId() && getCheckAddress() && getCheckPhonenumber() && getCheckGmail()&& getCheckDatebirth() && getCheckIdentitycard())
    {
        QSqlQuery query,qury;
        QString date_birth = getDatebirth();
        QDate date = QDate::fromString(date_birth,"dd-MM-yyyy");
        date_birth = date.toString("yyyy-MM-dd");
        qury.prepare("INSERT INTO IMAGE_USERS(userID) VALUES(:id)");
        qury.bindValue(0,getStudentId());
        query.prepare("INSERT INTO Student_DATABASE(MSSV,NAME,CLASSNAME,MAJOR,GMAIL,SEX,PHONENUMBER,ADDRESS,USERNAME,PASSWORD,CMND,DATEBIRTH) VALUES(:MSSV,:NAME,:CLASSNAME,:MAJOR,:GMAIL,:SEX,:PHONENUMBER,:ADDRESS,:USERNAME,:PASSWORD,:CMND,CAST(:DATEBIRTH AS DATE))");
        query.bindValue(0,getStudentId());
        query.bindValue(1,getName());
        query.bindValue(2,getClassname());
        query.bindValue(3,getMajor());
        query.bindValue(4,getGmail());
        query.bindValue(5,getSex());
        query.bindValue(6,getPhonenumber());
        query.bindValue(7,getAddress());
        query.bindValue(8,getUsername());
        query.bindValue(9,getPassword());
        query.bindValue(10,getIdentitycard());
        query.bindValue(11,date_birth);
        if (query.exec() && qury.exec())
        {
            messageBoxOneButton("Thông báo !","Tạo tài khoản thành công !","Đồng ý",300,150,80,28);
        }
    }
    else
    {
        messageBoxOneButton("Thông báo !","Tạo tài khoản thất bại !","Đồng ý",300,150,80,28);
    }
};


//check data is duplicated or not.
bool Student_LogIn_Form::checkForDuplicateData()
{

    QString _cmnd = "";
    QString _mssv = "";
    QString _gmail = "";
    QString _username = "";
    QString _phonenumber = "";
    QString resultStr = "";
    int i =0;bool nl = false;

    QSqlQuery sqlquery,querylec,qury;
    sqlquery.prepare("SELECT MSSV,CMND,GMAIL,USERNAME,PHONENUMBER FROM Student_DATABASE");
    querylec.prepare("SELECT MSGV,CMND,GMAIL,USERNAME,PHONENUMBER FROM Lecturer_DATABASE");
    qury.prepare("SELECT MSSV,CMND,GMAIL,USERNAME,PHONENUMBER FROM Student_DATABASE WHERE MSSV='"+getStudentId()+"'");
    qury.exec();
    qury.first();
    if(qury.value(0).toString() == getStudentId() && qury.value(1).toString() == getIdentitycard() && qury.value(2).toString() == getGmail() && qury.value(3).toString() == getUsername() && qury.value(4).toString() == getPhonenumber())
    {nl = true;messageBoxOneButton("Thông báo !","Tài khoản này đã tồn tại !","Đồng ý",250,150,80,28);return true;}
    if(sqlquery.exec() && querylec.exec() && !nl)
    {
        while (sqlquery.next() || querylec.next())
        {
            if (getIdentitycard() == sqlquery.value(1).toString() || getIdentitycard() == querylec.value(1).toString()){_cmnd = "\t\tCăn cước công dân\n";ui->lineEdit_cmnds->setStyleSheet("color:red");++i;};
            if (getStudentId() == sqlquery.value(0).toString() || getStudentId() == querylec.value(0).toString()){_mssv = "\t\tMã số sinh viên\n";ui->lineEdit_mssvs->setStyleSheet("color:red");++i;};
            if (getGmail() == sqlquery.value(2).toString() || getGmail() == querylec.value(2).toString() ){_gmail = "\t\tEmail\n";ui->lineEdit_emails->setStyleSheet("color:red");++i;};
            if (getUsername() == sqlquery.value(3).toString() || getUsername() == querylec.value(3).toString()){_username = "\t\tTên tài khoản\n";ui->lineEdit_usernames->setStyleSheet("color:red");++i;};
            if (getPhonenumber() == sqlquery.value(4).toString() || getPhonenumber() == querylec.value(4).toString()){_phonenumber = "\t\tSố điện thoại\n";ui->lineEdit_phonenumbers->setStyleSheet("color:red");++i;};
        }
    }
    resultStr =_mssv + _gmail + _phonenumber + _cmnd + _username;

    if(!resultStr.isEmpty() && !nl)
    {
        messageBoxOneButton("Thông báo !",resultStr + "những thông tin trên của bạn đã được sử dụng !","Đồng ý",300,150,80,28);
        return true;
    }
    return false;
};

//NAME,MSSV,CLASSNAME,MAJOR,GMAIL,SEX,PHONENUMBER,ADDRESS,USERNAME,PASSWORD,CMND,DATEBIRTH


void Student_LogIn_Form::doNothing()
{
    qDebug()<<"DO NOTHING";
}

void Student_LogIn_Form::closeWindow()
{
    this->close();
}


//check phone number is valid or not
void Student_LogIn_Form::on_lineEdit_phonenumbers_editingFinished()
{
    QRegularExpression rec("[0]{1}[0-9]{9}");
    QString phone_number = ui->lineEdit_phonenumbers->text();
    QRegularExpressionMatch rec_math = rec.match(phone_number);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QSqlQuery querystu,querylec;
        querystu.prepare("SELECT PHONENUMBER FROM Student_DATABASE WHERE PHONENUMBER = '"+ui->lineEdit_phonenumbers->text()+"'");
        querylec.prepare("SELECT PHONENUMBER FROM Lecturer_DATABASE WHERE PHONENUMBER = '"+ui->lineEdit_phonenumbers->text()+"'");
        querystu.exec();
        querylec.exec();
        querystu.first();
        querylec.first();
        if (querystu.value(0).toString() == ui->lineEdit_phonenumbers->text() || querylec.value(0).toString() == ui->lineEdit_phonenumbers->text())
        {
           QToolTip::hideText();
           QPalette pal;
           pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
           QToolTip::showText(ui->lineEdit_phonenumbers->mapToGlobal(QPoint()),tr("Số điện thoại đã được sử dụng !"));
           QToolTip::setPalette(pal);
           ui->lineEdit_phonenumbers->setStyleSheet("color : red");
           //this->checkPhonenumber = false;
           this->setCheckPhonenumber(false);
        }
        else
        {
            QToolTip::hideText();
            ui->lineEdit_phonenumbers->setStyleSheet("color : black");
            //this->checkPhonenumber = true;
            this->setCheckPhonenumber(true);
        }
    }
    if(!it)
    {
        QToolTip::hideText();
        ui->lineEdit_phonenumbers->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_phonenumbers->mapToGlobal(QPoint()), tr("Số điện thoại không hợp lệ !"));
        QToolTip::setPalette(pal);
        //this->checkPhonenumber = false;
        this->setCheckPhonenumber(false);
    }
}


//check email is valid or not
void Student_LogIn_Form::on_lineEdit_emails_editingFinished()
{
    //\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b
    QRegularExpression rec("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",QRegularExpression::CaseInsensitiveOption);
    QString emaill = ui->lineEdit_emails->text();
    QRegularExpressionMatch rec_math = rec.match(emaill);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QSqlQuery querystu,querylec;
        querystu.prepare("SELECT GMAIL FROM Student_DATABASE WHERE GMAIL = '"+ui->lineEdit_emails->text()+"'");
        querylec.prepare("SELECT GMAIL FROM Lecturer_DATABASE WHERE GMAIL = '"+ui->lineEdit_emails->text()+"'");
        querystu.exec();
        querylec.exec();
        querystu.first();
        querylec.first();
        if (querystu.value(0).toString() == ui->lineEdit_emails->text() || querylec.value(0).toString() == ui->lineEdit_emails->text())
        {
           QToolTip::hideText();
           QPalette pal;
           pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
           QToolTip::showText(ui->lineEdit_emails->mapToGlobal(QPoint()),tr("Email đã được sử dụng ! "));
           QToolTip::setPalette(pal);
           ui->lineEdit_emails->setStyleSheet("color : red");
           //this->checkGmail = false;
           this->setCheckGmail(false);
        }
        else
        {
            QToolTip::hideText();
            ui->lineEdit_emails->setStyleSheet("color : black");
            //this->checkGmail = true;
            this->setCheckGmail(true);
        }
    }
    if(!it)
    {
        QToolTip::hideText();
        ui->lineEdit_emails->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_emails->mapToGlobal(QPoint()), tr("Email không hợp lệ !"));
        QToolTip::setPalette(pal);
        //this->checkGmail = false;
        this->setCheckGmail(false);
    }
}


//check identity card is valid or not
void Student_LogIn_Form::on_lineEdit_cmnds_editingFinished()
{
    QRegularExpression rec("([0-9]{9})");
    QString cmnd = ui->lineEdit_cmnds->text();
    QRegularExpressionMatch rec_match = rec.match(cmnd);
    bool math_return = rec_match.hasMatch();
    if (math_return)
    {
        QSqlQuery querystu,querylec;
        querystu.prepare("SELECT CMND FROM Student_DATABASE WHERE CMND = '"+ui->lineEdit_cmnds->text()+"'");
        querylec.prepare("SELECT CMND FROM Lecturer_DATABASE WHERE CMND = '"+ui->lineEdit_cmnds->text()+"'");
        querystu.exec();
        querylec.exec();
        querystu.first();
        querylec.first();
        if (querystu.value(0).toString() == ui->lineEdit_cmnds->text() || querylec.value(0).toString() == ui->lineEdit_cmnds->text())
        {
           QToolTip::hideText();
           QPalette pal;
           pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
           QToolTip::showText(ui->lineEdit_cmnds->mapToGlobal(QPoint()),tr("Căn cước đã được sử dụng !"));
           QToolTip::setPalette(pal);
           ui->lineEdit_cmnds->setStyleSheet("color : red");
           this->setCheckIdentitycard(false);
        }
        else
        {
            QToolTip::hideText();
            ui->lineEdit_cmnds->setStyleSheet("color : black");
            this->setCheckIdentitycard(true);
        }
    }
    if(!math_return)
    {
        QToolTip::hideText();
        ui->lineEdit_cmnds->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_cmnds->mapToGlobal(QPoint()), tr("Căn cước không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->setCheckIdentitycard(false);
    }
}



void Student_LogIn_Form::closeEvent(QCloseEvent *event)
{
    ui->lineEdit_addresss->clear();
    ui->lineEdit_classnames->clear();
    ui->lineEdit_cmnds->clear();
    ui->lineEdit_datebirths->clear();
    ui->lineEdit_emails->clear();
    ui->lineEdit_majors->clear();
    ui->lineEdit_mssvs->clear();
    ui->lineEdit_names->clear();
    ui->lineEdit_passwords->clear();
    ui->lineEdit_phonenumbers->clear();
    ui->lineEdit_usernames->clear();
    ui->lineEdit_addresss->setStyleSheet("color : black");
    ui->lineEdit_classnames->setStyleSheet("color : black");
    ui->lineEdit_cmnds->setStyleSheet("color : black");
    ui->lineEdit_datebirths->setStyleSheet("color : black");
    ui->lineEdit_emails->setStyleSheet("color : black");
    ui->lineEdit_majors->setStyleSheet("color : black");
    ui->lineEdit_mssvs->setStyleSheet("color : black");
    ui->lineEdit_names->setStyleSheet("color : black");
    ui->lineEdit_passwords->setStyleSheet("color : black");
    ui->lineEdit_phonenumbers->setStyleSheet("color : black");
    ui->lineEdit_usernames->setStyleSheet("color : black");
    event->accept();
}

void Student_LogIn_Form::on_pushButton_exits_clicked()
{
    this->close();
}


void Student_LogIn_Form::on_pushButton_sig_clicked()
{
    getInformation();
}

void Student_LogIn_Form::keyReleaseEvent(QKeyEvent *event)
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

//check datebirth is valid or not
void Student_LogIn_Form::on_lineEdit_datebirths_editingFinished()
{
    string dater= ui->lineEdit_datebirths->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_datebirths->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_datebirths->text(),"dd-MM-yyyy");
    if (!date.isValid())
    {
        QToolTip::hideText();
        ui->lineEdit_datebirths->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_datebirths->mapToGlobal(QPoint()),tr("Ngày sinh không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->setCheckDatebirth(false);
    }
    if (date.isValid())
    {
        ui->lineEdit_datebirths->setStyleSheet("color : black");
        this->setCheckDatebirth(true);
    }
}


//check username is valid or not.
void Student_LogIn_Form::on_lineEdit_usernames_editingFinished()
{
    QSqlQuery query,querylec;
    query.prepare("SELECT USERNAME FROM Student_DATABASE WHERE USERNAME = '"+ui->lineEdit_usernames->text()+"'");
    querylec.prepare("SELECT USERNAME FROM Lecturer_DATABASE WHERE USERNAME = '"+ui->lineEdit_usernames->text()+"'");
    query.exec();
    query.first();
    querylec.exec();
    querylec.first();
    if (ui->lineEdit_usernames->text().isEmpty())
    {
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_usernames->mapToGlobal(QPoint()),tr("Tên tài khoản không hợp lệ !"));
        QToolTip::setPalette(pal);
        ui->lineEdit_usernames->setStyleSheet("color : red");
        this->setCheckUsername(false);
    }
    else if (query.value(0).toString() == ui->lineEdit_usernames->text() || querylec.value(0).toString() == ui->lineEdit_usernames->text())
    {
       QToolTip::hideText();
       QPalette pal;
       pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
       QToolTip::showText(ui->lineEdit_usernames->mapToGlobal(QPoint()),tr("Tên tài khoản đã được sử dụng !"));
       QToolTip::setPalette(pal);
       ui->lineEdit_usernames->setStyleSheet("color : red");
       this->setCheckUsername(false);
    }
    else
    {
        QToolTip::hideText();
        ui->lineEdit_usernames->setStyleSheet("color : black");
        this->setCheckUsername(true);
    }
}

//check address is valid or not
void Student_LogIn_Form::on_lineEdit_addresss_editingFinished()
{
    QRegularExpression rec("[a-zA-Z]",QRegularExpression::CaseInsensitiveOption);
    QString place = ui->lineEdit_addresss->text();
    QRegularExpressionMatch rec_math = rec.match(place);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QToolTip::hideText();
        ui->lineEdit_addresss->setStyleSheet("color : black");
        this->setCheckAddress(true);
        stringstream ss;
        string name="",temp="",str;
        QString name_ = ui->lineEdit_addresss->text();
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
        ui->lineEdit_addresss->clear();
        name.erase(name.size()-1);
        ui->lineEdit_addresss->setText(name.c_str());
    }
    if(!it)
    {
        QToolTip::hideText();
        ui->lineEdit_addresss->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_addresss->mapToGlobal(QPoint()), tr("Địa chỉ không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->setCheckAddress(false);
    }
}

bool Student_LogIn_Form::checkClassExist(QString& classNamee)
{
    string str = classNamee.toStdString();
    for (unsigned long long i = 0;i < str.size();i++ )
    {
        if (isalpha(str[i]) && str[i] >= 97 && str[i] <= 122)
        {
            str[i]=str[i]-32;
        }
    }
    qDebug()<<str.c_str();
    QSqlQuery qry;
    QString input = str.c_str();
    str.clear();
    qry.prepare("SELECT * FROM _thuocKhoa");
    qry.exec();
    while(qry.next())
    {
        qDebug()<<qry.value(0).toString();
        if (qry.value(0).toString() == input)
        {
            return true;
        }
    }
    return false;
}

void Student_LogIn_Form::editingStudentIDFinished()
{
    QString studenid = ui->lineEdit_mssvs->text();
    QSqlQuery qury;
    qury.prepare("SELECT MSSV,CLASSNAME,MAJOR,tenSinhVien FROM Student WHERE MSSV='"+studenid+"'");
    qury.exec();
    qury.first();
    ui->lineEdit_classnames->setText(qury.value(1).toString());
    ui->lineEdit_majors->setText(qury.value(2).toString());
    ui->lineEdit_names->setText(qury.value(3).toString());
    ui->lineEdit_majors->setCursorPosition(0);
    ui->lineEdit_names->setCursorPosition(0);
}

bool Student_LogIn_Form::checkStudentIDExistInSystem()
{
    QSqlQuery qury;
    QString id = ui->lineEdit_mssvs->text();
    qury.prepare("SELECT MSSV FROM Student WHERE MSSV=:mssv");
    qury.bindValue(0,id);
    qury.exec();
    qury.first();
    if(qury.value(0).toString() == id){return true;}
    return false;
}

void Student_LogIn_Form::enterEventSignUp()
{
    getInformation();
}

void Student_LogIn_Form::initPasswordHint()
{
    ui->lineEdit_passwords->setEchoMode(QLineEdit::Password);
    QAction* action = ui->lineEdit_passwords->addAction(QIcon(":/icon-file/invisible.png"),QLineEdit::TrailingPosition);
    button = qobject_cast<QToolButton*>(action->associatedWidgets().last());
    button->setCursor(QCursor(Qt::PointingHandCursor));
    //button->show();
    button->hide();
    connect(button,&QToolButton::pressed,this,&Student_LogIn_Form::passwordShow);
    connect(button,&QToolButton::released,this,&Student_LogIn_Form::passwordHint);
}

void Student_LogIn_Form::passwordShow()
{
    QToolButton* button = qobject_cast<QToolButton*>(sender());
    button->setIcon(QIcon(":/icon-file/password-Hint.png"));
    ui->lineEdit_passwords->setEchoMode(QLineEdit::Normal);
}
void Student_LogIn_Form::passwordHint()
{
    QToolButton* button = qobject_cast<QToolButton*>(sender());
    button->setIcon(QIcon(":/icon-file/invisible.png"));
    ui->lineEdit_passwords->setEchoMode(QLineEdit::Password);
}

void Student_LogIn_Form::showButton(){button->show();}

void Student_LogIn_Form::hideButton(){button->hide();}


void Student_LogIn_Form::on_lineEdit_mssvs_textChanged(const QString &arg1)
{
    QRegularExpression rec("([0-9]{10})",QRegularExpression::CaseInsensitiveOption);
    QString mssv = ui->lineEdit_mssvs->text();
    QRegularExpressionMatch rec_match = rec.match(mssv);
    bool match_rec = rec_match.hasMatch();
    if (match_rec)
    {
        QSqlQuery querystu,querylec;
        querystu.prepare("SELECT MSSV FROM Student_DATABASE WHERE MSSV = '"+ui->lineEdit_mssvs->text()+"'");
        querylec.prepare("SELECT MSGV FROM Lecturer_DATABASE WHERE MSGV = '"+ui->lineEdit_mssvs->text()+"'");
        querystu.exec();
        querylec.exec();
        querystu.first();
        querylec.first();
        if (querystu.value(0).toString() == ui->lineEdit_mssvs->text() || querylec.value(0).toString() == ui->lineEdit_mssvs->text())
        {
           QToolTip::hideText();
           QPalette pal;
           pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
           QToolTip::showText(ui->lineEdit_mssvs->mapToGlobal(QPoint()),tr("Mã sinh viên đã được sử dụng !"));
           QToolTip::setPalette(pal);
           ui->lineEdit_mssvs->setStyleSheet("color : red");
           this->setCheckStudenId(false);
        }
        else
        {
            if(!checkStudentIDExistInSystem())
            {
                QToolTip::hideText();
                QPalette pal;
                pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
                QToolTip::setPalette(pal);
                ui->lineEdit_mssvs->setStyleSheet("color : red");
                QToolTip::showText(ui->lineEdit_mssvs->mapToGlobal(QPoint()),tr("Bạn chưa được ghi danh vào hệ thống lớp của trường !"));
            }
            else
            {
                QToolTip::hideText();
                ui->lineEdit_mssvs->setStyleSheet("color : black");
                this->setCheckStudenId(true);
                string id = ui->lineEdit_mssvs->text().toStdString();
                for (unsigned long long i =0 ;i<id.size() ;++i ) {
                    if (isalpha(id[i]) && islower(id[i])) {
                        id[i]=id[i]-32;
                    }
                }
                ui->lineEdit_mssvs->setText(id.c_str());
            }
        }
    }
    else
    {
        QToolTip::hideText();
        ui->lineEdit_mssvs->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_mssvs->mapToGlobal(QPoint()),tr("Mã sinh viên không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->setCheckStudenId(false);
    }
}

