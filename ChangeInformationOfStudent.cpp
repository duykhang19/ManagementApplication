#include "ChangeInformationOfStudent.h"
#include "ui_ChangeInformationOfStudent.h"
#include<string>
#include<sstream>
#include<iostream>

using namespace std;

//Default Constructors.
ChangeInformationOfStudent::ChangeInformationOfStudent(QWidget *parent,int a) :
    QWidget(parent),
    ui(new Ui::ChangeInformationOfStudent)
{
    ui->setupUi(this);
    b = a;
    QObject::connect(ui->lineEdit_id,&QLineEdit::textChanged,this,&ChangeInformationOfStudent::checkIdIsValid);
}

void ChangeInformationOfStudent::showLineEditClass()
{
    ui->label->show();ui->lineEdit_class->show();
}

void ChangeInformationOfStudent::hideLineEditClass()
{
    ui->label->hide();ui->lineEdit_class->hide();
}

void ChangeInformationOfStudent::setLName(QString a)
{
    ui->lineEdit_name->setText(a);
}

void ChangeInformationOfStudent::setLMajor(QString a)
{
    ui->lineEdit_major->setText(a);
}

void ChangeInformationOfStudent::setLId(QString a)
{
    ui->lineEdit_id->setText(a);
}

void ChangeInformationOfStudent::setLClass(QString a)
{
    ui->lineEdit_class->setText(a);
}

void ChangeInformationOfStudent::setOldId(QString a)
{
    this->oldID = a;
}

bool ChangeInformationOfStudent::checkIdIsValid()
{
    qDebug()<<"DCMMM-> "<<b;
    QSqlQuery qury;
    if(b==1){
        qury.prepare("SELECT MSGV FROM Teacher WHERE MSGV='"+ui->lineEdit_id->text()+"'");
    }
    if(b==2){
        qury.prepare("SELECT MSSV FROM Student WHERE MSSV='"+ui->lineEdit_id->text()+"'");
    }
    qury.exec();qury.first();
    if(b==1){
        if(qury.value(0).toString() == ui->lineEdit_id->text()){
            QToolTip::hideText();
            QPalette pal;
            pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(ui->lineEdit_id->mapToGlobal(QPoint()),tr("Mã giảng viên đã tồn tại !"));
            QToolTip::setPalette(pal);
            this->check = false;
            ui->lineEdit_id->setStyleSheet("color : red");
            return false;
        }
        this->check = true;
        ui->lineEdit_id->setStyleSheet("color : black");
        return true;
    }
    if(b==2){
        if(qury.value(0).toString() == ui->lineEdit_id->text()){
            QToolTip::hideText();
            QPalette pal;
            pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(ui->lineEdit_id->mapToGlobal(QPoint()),tr("Mã sinh viên đã tồn tại !"));
            QToolTip::setPalette(pal);
            this->check = false;
            ui->lineEdit_id->setStyleSheet("color : red");
            return false;
        }
        this->check = true;
        ui->lineEdit_id->setStyleSheet("color : black");
        return true;
    }
    this->check = false;
    return false;
}

void ChangeInformationOfStudent::setB(int a)
{
    this->b = a;
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
    ui->lineEdit_id->clear();ui->lineEdit_major->clear();ui->lineEdit_name->clear();
    emit destroyed(this);
    this->close();
}

//function to update information of user (CLICK).
void ChangeInformationOfStudent::on_pushButton_update_clicked()
{
    QSqlQuery query;
    if(b==1 && check){
        query.prepare("EXEC _UPDATETEACHER '"+ui->lineEdit_id->text()+"',N'"+ui->lineEdit_name->text()+"',N'"+ui->lineEdit_major->text()+"','"+oldID+"'");
        if(query.exec() && !ui->lineEdit_id->text().isEmpty() && !ui->lineEdit_name->text().isEmpty() && !ui->lineEdit_major->text().isEmpty()){
            messageBoxOneButton("Cập nhật","Cập nhật thành công !","Đồng ý",200,150,80,25);
            emit destroyed(this);
            this->close();
        }
        else{
            messageBoxOneButton("Cập nhật","Cập nhật thất bại !","Đồng ý",200,150,80,25);
        }
    }
    else if(b==2 && check){
        query.prepare("EXEC _UPDATESTUDENT '"+ui->lineEdit_id->text()+"',N'"+ui->lineEdit_name->text()+"',N'"+ui->lineEdit_major->text()+"','"+ui->lineEdit_class->text()+"','"+oldID+"'");
        if(query.exec() && !ui->lineEdit_id->text().isEmpty() && !ui->lineEdit_name->text().isEmpty() && !ui->lineEdit_class->text().isEmpty() && !ui->lineEdit_major->text().isEmpty()){
            messageBoxOneButton("Cập nhật","Cập nhật thành công !","Đồng ý",200,150,80,25);
            emit destroyed(this);
            this->close();
        }
        else{
            messageBoxOneButton("Cập nhật","Cập nhật thất bại !","Đồng ý",200,150,80,25);
        }
    }
    else{messageBoxOneButton("Cập nhật","Cập nhật thất bại !","Đồng ý",200,150,80,25);}
}




