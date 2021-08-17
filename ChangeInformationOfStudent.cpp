#include "ChangeInformationOfStudent.h"
#include "ui_ChangeInformationOfStudent.h"
#include<string>
#include<sstream>
#include<iostream>

using namespace std;

//Default Constructors.
ChangeInformationOfStudent::ChangeInformationOfStudent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeInformationOfStudent)
{
    ui->setupUi(this);
    ui->lineEdit_date->setMaxLength(10);
    ui->lineEdit_phonenumber->setMaxLength(10);
    ui->lineEdit_cmnd->setMaxLength(9);
    ui->lineEdit_address->setMaxLength(25);
    ui->lineEdit_placebirth->setMaxLength(25);
}

//Destructors
ChangeInformationOfStudent::~ChangeInformationOfStudent()
{
    delete ui;
}

//fuction to receive Key_Up and Key_End.
void ChangeInformationOfStudent::keyReleaseEvent(QKeyEvent *event)
{
    event->accept();
    if (event->key() == Qt::Key_Up)
    {
        this->focusPreviousChild();
    }
    if (event->key() == Qt::Key_Down)
    {
        this->focusNextChild();
    }
}

//function to receive closeEvent.
void ChangeInformationOfStudent::closeEvent(QCloseEvent *event)
{
    ui->lineEdit_address->clear();
    ui->lineEdit_cmnd->clear();
    ui->lineEdit_date->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_ethnic->clear();
    ui->lineEdit_phonenumber->clear();
    ui->lineEdit_placebirth->clear();
    this->close();
}

//function to update information of user (CLICK).
void ChangeInformationOfStudent::on_pushButton_update_clicked()
{
    qDebug()<<getDistinguish();
    messageBoxOneButton("Cập nhật","Những vùng không hợp lệ sẽ không được cập nhật !","Đồng ý",200,150,80,25);
    QSqlQuery query;
    QString _datebirth = ui->lineEdit_date->text();
    QString _placeofbirth = ui->lineEdit_placebirth->text();
    QString _ethnic = ui->lineEdit_ethnic->text();
    QString _phonenumber = ui->lineEdit_phonenumber->text();
    QString _identitycard = ui->lineEdit_cmnd->text();
    QString _address = ui->lineEdit_address->text();
    QString _email = ui->lineEdit_email->text();
    QString _sex;

    if (ui->radioButton_male->isChecked())
    {
        _sex = "Nam";
    }
    if (ui->radioButton_female->isChecked())
    {
        _sex = "Nữ";
    }
    QString date_birth = ui->lineEdit_date->text();
    QDate date = QDate::fromString(date_birth,"dd-MM-yyyy");
    date_birth = date.toString("yyyy-MM-dd");

    if (!_datebirth.isEmpty())
    {   if (date.isValid() && getDistinguish() == 1)
        {
                query.prepare("UPDATE Student_DATABASE SET DATEBIRTH = CAST('"+date_birth+"' AS DATE) WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
                query.exec();
        }

        if (date.isValid() && getDistinguish() == 2)
        {
                query.prepare("UPDATE Lecturer_DATABASE SET DATEBIRTH = CAST('"+date_birth+"' AS DATE) WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
                query.exec();
        }
    }

    if (!_placeofbirth.isEmpty())
    {
        if (checkValidPlaceOfBirth() && getDistinguish() == 1)
        {
            query.prepare("UPDATE Student_DATABASE SET PLACEOFBIRTH = N'"+_placeofbirth+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
            query.exec();
        }

        if (checkValidPlaceOfBirth() && getDistinguish() == 2)
        {
            query.prepare("UPDATE Lecturer_DATABASE SET PLACEOFBIRTH = N'"+_placeofbirth+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
            query.exec();
        }
    }

    if (!_sex.isEmpty() && getDistinguish() == 1)
    {
      query.prepare("UPDATE Student_DATABASE SET SEX = N'"+_sex+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
      query.exec();
    }
    if (!_sex.isEmpty() && getDistinguish() == 2)
    {
      query.prepare("UPDATE Lecturer_DATABASE SET SEX = N'"+_sex+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
      query.exec();
    }

    if (!_ethnic.isEmpty())
    {
        if (checkValidEthnic() && getDistinguish() == 1)
        {
            query.prepare("UPDATE Student_DATABASE SET ETHNIC = N'"+_ethnic+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
            query.exec();
        }

        if (checkValidEthnic() && getDistinguish() == 2)
        {
            query.prepare("UPDATE Lecturer_DATABASE SET ETHNIC = N'"+_ethnic+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
            query.exec();
        }
    }
    if (!_phonenumber.isEmpty())
    {
        if (checkValidPhoneNumber() && getDistinguish() == 1)
        {
            query.prepare("UPDATE Student_DATABASE SET PHONENUMBER = '"+_phonenumber+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
            query.exec();
        }

        if (checkValidPhoneNumber() && getDistinguish() == 2)
        {
            query.prepare("UPDATE Lecturer_DATABASE SET PHONENUMBER = '"+_phonenumber+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
            query.exec();
        }
    }
    if (!_identitycard.isEmpty())
    {
        if (checkValidIdentityCard() && getDistinguish() == 1)
        {
            query.prepare("UPDATE Student_DATABASE SET CMND = '"+_identitycard+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
            query.exec();
        }

        if (checkValidIdentityCard() && getDistinguish() == 2)
        {
            query.prepare("UPDATE Lecturer_DATABASE SET CMND = '"+_identitycard+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
            query.exec();
        }
    }
   if (!_address.isEmpty())
   {
       if (checkValidAddress() && getDistinguish() == 1)
       {
           query.prepare("UPDATE Student_DATABASE SET ADDRESS = N'"+_address+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
           query.exec();
       }

       if (checkValidAddress() && getDistinguish() == 2)
       {
           query.prepare("UPDATE Lecturer_DATABASE SET ADDRESS = N'"+_address+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
           query.exec();
       }
   }
   if (!_email.isEmpty())
   {
       if (checkValidEmail() && getDistinguish() == 1)
       {
           query.prepare("UPDATE Student_DATABASE SET GMAIL = '"+_email+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
           query.exec();
       }

       if (checkValidEmail() && getDistinguish() == 2)
       {
           query.prepare("UPDATE Lecturer_DATABASE SET GMAIL = '"+_email+"' WHERE USERNAME = '"+username+"' AND PASSWORD = '"+password+"'");
           query.exec();
       }
   }
   emit destroyed(this);
   this->close();
}



//function to receive username and password of user from class Student_Interactive_Interface.
void ChangeInformationOfStudent::getUserPass(QString user, QString pass)
{
    this->username = user;this->password = pass;
}

//get place of birth from lineEdit_placeofbirth
QString ChangeInformationOfStudent::getplaceofbirth()
{
    return placeofbirth;
}
//get place of birth from lineEdit_ethnic
QString ChangeInformationOfStudent::getethnic()
{
    return ethnic;
}
//get place of birth from lineEdit_address
QString ChangeInformationOfStudent::getaddress()
{
    return address;
}

//function to check lineEdit_date is valid or invalid.
void ChangeInformationOfStudent::on_lineEdit_date_editingFinished()
{
    string dater= ui->lineEdit_date->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_date->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_date->text(),"dd-MM-yyyy");
    if (!date.isValid())
    {
        QToolTip::hideText();
        ui->lineEdit_date->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_date->mapToGlobal(QPoint()),tr("Invalid Date"));
        QToolTip::setPalette(pal);
    }
    if (date.isValid())
    {
        ui->lineEdit_date->setStyleSheet("color : black");
    }
}


//function to check phone number is valid or not return true if valid.
bool ChangeInformationOfStudent::checkValidPhoneNumber()
{
    QRegularExpression rec("^[0]{1}[0-9]{9}");
    QString phone_number = ui->lineEdit_phonenumber->text();
    QRegularExpressionMatch rec_math = rec.match(phone_number);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QSqlQuery query_phonenumber,querylec;
        QString phone = ui->lineEdit_phonenumber->text();
        query_phonenumber.prepare("SELECT PHONENUMBER FROM Student_DATABASE WHERE PHONENUMBER = '"+ui->lineEdit_phonenumber->text()+"'");
        querylec.prepare("SELECT PHONENUMBER FROM Lecturer_DATABASE WHERE PHONENUMBER = '"+ui->lineEdit_phonenumber->text()+"'");
        query_phonenumber.exec();
        querylec.exec();
        query_phonenumber.first();
        querylec.first();
        if (query_phonenumber.value(0).toString() == phone || querylec.value(0).toString() == phone)
        {
            QToolTip::hideText();
            QPalette pall;
            pall.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(ui->lineEdit_phonenumber->mapToGlobal(QPoint()), tr("Số điện thoại đã được sử dụng !"));
            QToolTip::setPalette(pall);
            ui->lineEdit_phonenumber->setStyleSheet("color : red");
            return false;
        }
        if (query_phonenumber.value(0).toString() != phone)
        {
            QToolTip::hideText();
            ui->lineEdit_phonenumber->setStyleSheet("color : black");
            return true;
        }
    }
    ui->lineEdit_phonenumber->setStyleSheet("color : red");
    return false;
}


//function to check Identity Card is valid or not return true if valid.
bool ChangeInformationOfStudent::checkValidIdentityCard()
{
    QRegularExpression rec;
    rec.setPattern("[0-9]{9}");
    QString cmnd = ui->lineEdit_cmnd->text();
    QRegularExpressionMatch rec_match = rec.match(cmnd);
    bool check = rec_match.hasMatch();
    if (check)
    {
        QSqlQuery query,querylec;
        query.prepare("SELECT CMND FROM Student_DATABASE WHERE CMND = '"+ui->lineEdit_cmnd->text()+"'");
        querylec.prepare("SELECT CMND FROM Lecturer_DATABASE WHERE CMND = '"+ui->lineEdit_cmnd->text()+"'");
        query.exec();
        querylec.exec();
        query.first();
        querylec.first();
        if (query.value(0).toString() == ui->lineEdit_cmnd->text() || querylec.value(0).toString() == ui->lineEdit_cmnd->text())
        {
            QToolTip::hideText();
            QPalette pall;
            pall.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(ui->lineEdit_cmnd->mapToGlobal(QPoint()), tr("Căn cước đã được sử dụng !"));
            QToolTip::setPalette(pall);
            ui->lineEdit_cmnd->setStyleSheet("color : red");
            return false;
        }
        else
        {
            QToolTip::hideText();
            ui->lineEdit_cmnd->setStyleSheet("color : black");
            return true;
        }
    }
    ui->lineEdit_cmnd->setStyleSheet("color : red");
    return false;
}


//function to check email is valid or not return true if valid.
bool ChangeInformationOfStudent::checkValidEmail()
{
    QRegularExpression rec("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",QRegularExpression::CaseInsensitiveOption);
    QString emaill = ui->lineEdit_email->text();
    QRegularExpressionMatch rec_math = rec.match(emaill);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QSqlQuery query,querylec;
        query.prepare("SELECT GMAIL FROM Student_DATABASE WHERE GMAIL = '"+ui->lineEdit_email->text()+"'");
        querylec.prepare("SELECT GMAIL FROM Lecturer_DATABASE WHERE GMAIL = '"+ui->lineEdit_email->text()+"'");
        query.exec();
        querylec.exec();
        query.first();
        querylec.first();
        if (query.value(0).toString() == ui->lineEdit_email->text() || querylec.value(0).toString() == ui->lineEdit_email->text())
        {
            QToolTip::hideText();
            QPalette pall;
            pall.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(ui->lineEdit_email->mapToGlobal(QPoint()), tr("Email đã được sử dụng !"));
            QToolTip::setPalette(pall);
            ui->lineEdit_email->setStyleSheet("color : red");
            return false;
        }
        else
        {   QToolTip::hideText();
            ui->lineEdit_email->setStyleSheet("color : black");
            return true;
        }
    }
    ui->lineEdit_email->setStyleSheet("color : red");
    return false;
}

//function to check placeofbirth is valid or not.
void ChangeInformationOfStudent::on_lineEdit_placebirth_editingFinished()
{
    QRegularExpression rec("[a-zA-Z]",QRegularExpression::CaseInsensitiveOption);
    QString place = ui->lineEdit_placebirth->text();
    QRegularExpressionMatch rec_math = rec.match(place);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QToolTip::hideText();
        ui->lineEdit_placebirth->setStyleSheet("color : black");
        stringstream ss;
        string name="",temp="",str;
        QString name_ = ui->lineEdit_placebirth->text();
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
        ui->lineEdit_placebirth->clear();
        name.erase(name.size()-1);
        ui->lineEdit_placebirth->setText(name.c_str());
    }
    if(!it)
    {
        QToolTip::hideText();
        ui->lineEdit_placebirth->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_placebirth->mapToGlobal(QPoint()), tr("Không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
}


bool ChangeInformationOfStudent::checkValidPlaceOfBirth()
{
    QRegularExpression rec("[a-zA-Z]",QRegularExpression::CaseInsensitiveOption);
    QString place = ui->lineEdit_placebirth->text();
    QRegularExpressionMatch rec_math = rec.match(place);
    bool it = rec_math.hasMatch();
    if (it)
    {
        return true;
    }
    return false;
}

void ChangeInformationOfStudent::setDistinguish(int a)
{
    this->indexForDistinguish = a;
}

int ChangeInformationOfStudent::getDistinguish()
{
    return this->indexForDistinguish;
}

void ChangeInformationOfStudent::setIdd(QString a)
{
    this->id = a;
}

QString ChangeInformationOfStudent::getIdd()
{
    return this->id;
}

//function to check ethnic is valid or not.
void ChangeInformationOfStudent::on_lineEdit_ethnic_editingFinished()
{
    QRegularExpression rec("[a-zA-Z]",QRegularExpression::CaseInsensitiveOption);
    QString place = ui->lineEdit_ethnic->text();
    QRegularExpressionMatch rec_math = rec.match(place);
    bool it = rec_math.hasMatch();
    if (it)
    {
        QToolTip::hideText();
        ui->lineEdit_ethnic->setStyleSheet("color : black");
        stringstream ss;
        string name="",temp="",str;
        QString name_ = ui->lineEdit_ethnic->text();
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
        ui->lineEdit_ethnic->clear();
        name.erase(name.size()-1);
        ui->lineEdit_ethnic->setText(name.c_str());
    }
    if(!it)
    {
        QToolTip::hideText();
        ui->lineEdit_ethnic->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_ethnic->mapToGlobal(QPoint()), tr("Không hợp lệ"));
        QToolTip::setPalette(pal);
    }
}


bool ChangeInformationOfStudent::checkValidEthnic()
{
    QRegularExpression rec("[a-zA-Z]",QRegularExpression::CaseInsensitiveOption);
    QString place = ui->lineEdit_ethnic->text();
    QRegularExpressionMatch rec_math = rec.match(place);
    bool it = rec_math.hasMatch();
    if (it)
    {
        return true;
    }
    return false;
}
//function to check address is valid or not.
void ChangeInformationOfStudent::on_lineEdit_address_editingFinished()
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

    }
    if(!it)
    {
        QToolTip::hideText();
        ui->lineEdit_address->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_address->mapToGlobal(QPoint()), tr("Không hợp lệ"));
        QToolTip::setPalette(pal);
    }
}

bool ChangeInformationOfStudent::checkValidAddress()
{
    QRegularExpression rec("[a-zA-Z]",QRegularExpression::CaseInsensitiveOption);
    QString place = ui->lineEdit_address->text();
    QRegularExpressionMatch rec_math = rec.match(place);
    bool it = rec_math.hasMatch();
    if (it)
    {
        return true;
    }
    return false;
}

void ChangeInformationOfStudent::on_pushButton_addPic_clicked()
{
    QSqlQuery query;
    QSqlQuery getStudentId,delPic;
    QString MSSV;
    if(getDistinguish() == 1)
    {
        getStudentId.prepare("SELECT MSSV FROM Student_DATABASE WHERE USERNAME = :USERNAME");
    }
    if(getDistinguish() == 2)
    {
        getStudentId.prepare("SELECT MSGV FROM Lecturer_DATABASE WHERE USERNAME = :USERNAME");
    }
    getStudentId.bindValue(0,username);
    getStudentId.exec();
    getStudentId.first();
    MSSV = getStudentId.value(0).toString();
    QString filter = QString("JPG (*.jpg ,*.jpeg)");
    QString filePathName = QFileDialog::getOpenFileName(this,"Select a piture",NULL,filter);

    if (!filePathName.isEmpty())
    {
        delPic.prepare("DELETE FROM IMAGE_USERS WHERE userID = :mssv");
        delPic.bindValue(0,MSSV);
        delPic.exec();
        query.prepare("INSERT INTO IMAGE_USERS(userID,pic_personal) VALUES	(:MSSV,:fileName)");
        query.bindValue(0,MSSV);
        query.bindValue(1,filePathName.toUtf8());
        if(query.exec()){
            messageBoxOneButton("Thông báo !","Thêm ảnh đại diện thành công !","Đồng ý",250,150,80,28);
        }
    }
    emit destroyed();
}

void ChangeInformationOfStudent::on_lineEdit_phonenumber_textChanged(const QString &arg1)
{
    QRegularExpression rec("^[0]{1}[0-9]{9}");
    QString phone_number = ui->lineEdit_phonenumber->text();
    QRegularExpressionMatch rec_math = rec.match(phone_number);
    bool it = rec_math.hasMatch();
    if (it)
    {
        if (checkValidPhoneNumber())
        {
            QToolTip::hideText();
            ui->lineEdit_phonenumber->setStyleSheet("color : black");
        }
        if (!checkValidPhoneNumber())
        {
            QToolTip::hideText();
            QPalette pall;
            pall.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(ui->lineEdit_phonenumber->mapToGlobal(QPoint()), tr("Số điện thoại đã được sử dụng !"));
            QToolTip::setPalette(pall);
            ui->lineEdit_phonenumber->setStyleSheet("color : red");
        }
    }
    if(!it)
    {
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_phonenumber->mapToGlobal(QPoint()), tr("Số điện thoại không hợp lệ !"));
        QToolTip::setPalette(pal);
        ui->lineEdit_phonenumber->setStyleSheet("color : red");
    }
}

void ChangeInformationOfStudent::on_lineEdit_cmnd_textChanged(const QString &arg1)
{
    QRegularExpression rec;
    rec.setPattern("[0-9]{9}");
    QString cmnd = ui->lineEdit_cmnd->text();
    QRegularExpressionMatch rec_match = rec.match(cmnd);
    bool check = rec_match.hasMatch();
    if (check)
    {
        if (!checkValidIdentityCard())
        {
            QToolTip::hideText();
            QPalette pall;
            pall.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(ui->lineEdit_cmnd->mapToGlobal(QPoint()), tr("Căn cước đã được sử dụng !"));
            QToolTip::setPalette(pall);
            ui->lineEdit_cmnd->setStyleSheet("color : red");
        }
        else
        {
            QToolTip::hideText();
            ui->lineEdit_cmnd->setStyleSheet("color : black");
        }
    }
    if (!check)
    {
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_cmnd->mapToGlobal(QPoint()),tr("Căn cước không hợp lệ !"));
        QToolTip::setPalette(pal);
        ui->lineEdit_cmnd->setStyleSheet("color : red");
    }
}

void ChangeInformationOfStudent::on_lineEdit_email_textChanged(const QString &arg1)
{
    QRegularExpression rec("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",QRegularExpression::CaseInsensitiveOption);
    QString emaill = ui->lineEdit_email->text();
    QRegularExpressionMatch rec_math = rec.match(emaill);
    bool it = rec_math.hasMatch();
    if (it)
    {
        if (!checkValidEmail())
        {
            QToolTip::hideText();
            QPalette pall;
            pall.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(ui->lineEdit_email->mapToGlobal(QPoint()), tr("Email đã được sử dụng !"));
            QToolTip::setPalette(pall);
            ui->lineEdit_email->setStyleSheet("color : red");
        }
        else
        {
            QToolTip::hideText();
            ui->lineEdit_email->setStyleSheet("color : black");
        }
    }
    if(!it)
    {
        QToolTip::hideText();
        ui->lineEdit_email->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_email->mapToGlobal(QPoint()), tr("Email không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
}

void ChangeInformationOfStudent::on_pushButton_clicked()
{
    qDebug()<<"GET ID DD"<<getIdd();
    QSqlQuery qury;
    qury.prepare("DELETE  FROM IMAGE_USERS WHERE userID='"+getIdd()+"'");
    if(qury.exec()){messageBoxOneButton("Thông báo !","Xóa ảnh thành công !","Thoát",200,100,80,28);}else{messageBoxOneButton("Thông báo !","Xóa ảnh thất bại !","Thoát",200,100,80,28);}
    emit destroyed();
}

