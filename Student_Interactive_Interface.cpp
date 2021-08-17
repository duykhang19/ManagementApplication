#include "Student_Interactive_Interface.h"
#include "ui_Student_Interactive_Interface.h"
#include<QDebug>
#include<QString>
#include<QSqlQuery>
#include<QGraphicsScene>
#include<QDropEvent>
#include<QUrl>
#include<QSqlRecord>
#include<string>
#include<QDateEdit>
#include<QTextCharFormat>
#include<QtWidgets>
#include<QHeaderView>
#include<QToolBar>


Student_Interactive_Interface::Student_Interactive_Interface(QWidget *parent,bool pass,QString username, QString password) :
    QMainWindow(parent),
    ui(new Ui::Student_Interactive_Interface)
{
    ui->setupUi(this);
    setUsername(username);
    setPassWord(password);
    QSqlQuery getStudentId;
    QString MSSV;
    getStudentId.prepare("SELECT MSSV FROM Student_DATABASE WHERE USERNAME = :USERNAME");
    getStudentId.bindValue(0,getUsername());
    getStudentId.exec();
    getStudentId.first();
    MSSV = getStudentId.value(0).toString();
    setStuId(MSSV);
    index = 1;
    this->a = 0;
    wid = new QWidget();
    tableModelDebt = new QSqlTableModel();
    tableVieww = new QTableView();
    timer = new QTimer();
    this->indexForTableResA = -1;
    this->indexForTableResB = -1;
    qrymodall = new QSqlQueryModel();
    tableModel = new QSqlTableModel();
    tableModelRes = new QSqlTableModel();
    passWordChange = new ChangePassword(nullptr,getUsername(),getPassword());
    updateInformation = new ChangeInformationOfStudent();
    ui->tableView_mark->verticalHeader()->hide();
    comboxYear = new QComboBox();
    labelYear = new QLabel("Năm học :");
    conti = new QPushButton("Bắt đầu in");
    hLayout = new QHBoxLayout();vLayout = new QVBoxLayout();
    //
    Init();
    initGraph();
    calendar = new QCalendarWidget();
    widget = new QWidget();
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(UpdateTime()));
    timer->start(1000);
    QObject::connect(ui->tabWidget,&QTabWidget::currentChanged,this,&Student_Interactive_Interface::tabSelected);
    QObject::connect(calendar,&QCalendarWidget::selectionChanged,this,&Student_Interactive_Interface::onSelectionChanged);
    QObject::connect(updateInformation,&ChangeInformationOfStudent::destroyed,this,&Student_Interactive_Interface::REFRESH);
    QObject::connect(conti,&QPushButton::clicked,this,&Student_Interactive_Interface::printMark);
    ui->pushButton_information->setAutoDefault(true);
    ui->pushButton_mark->setAutoDefault(true);
    calendar->setGridVisible(true);
    inittabletime(QDate::currentDate());
    pullOfData();
    ui->tableView_registerCourse->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_registeredCourse->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableVieww->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableVieww->setWordWrap(true);
    updateDebtStudent();
    setPictureForUser();
    initComboBox();
    QLabel* label = new QLabel("");
    label->setFixedWidth(33);
    label->setFixedHeight(180);
    ui->toolBar->insertWidget(ui->actionTh_ng_tin_sinh_vi_n,label);
    this->setWindowFlags(Qt::Window);
    checkClose = true;
    setSaveOrNot(pass);
    readFile();
    //Graph
    ui->horizontalFrame->hide();
    ui->verticalSpacer_11->changeSize(20,540);
    initQMap();
}

Student_Interactive_Interface::~Student_Interactive_Interface()
{
    delete ui;
    delete timer;
    delete qrymodall;
    delete display;
    delete calendar;
    delete tableModel;
    delete tableModelRes;
    delete wid;
    delete updateInformation;
    delete tableModelDebt;
    delete tableVieww;
    delete passWordChange;
    delete chartView;delete widget;delete vLayout;delete hLayout;
    delete axis;delete chart;delete series;delete axisY;delete axisX;
    delete set0;delete set1;delete set2;delete set3;delete set4;delete set5;delete set6;delete set7;delete set8;delete set9;delete set10;
    delete comboxYear,delete labelYear,delete conti;
    mapp.clear();
}


//function to refresh data showed.
void Student_Interactive_Interface::REFRESH()
{
    ui->tabWidget->setCurrentWidget(ui->tab_infor);
    ui->label_placeofbirth->show();
    QSqlQuery query,qr;
    query.prepare("SELECT * FROM Student_DATABASE WHERE USERNAME = :username AND PASSWORD = :password");
    query.bindValue(0,getUsername());
    query.bindValue(1,getPassword());
    query.exec();
    query.first();
    qr.prepare("SELECT _thuocKhoa.tenKhoa FROM _thuocKhoa WHERE _thuocKhoa.tenLop=:tenLop");
    qr.bindValue(0,query.value(2).toString());
    qr.exec();
    qr.first();
    if (query.value(8).toString() == getUsername() && query.value(9).toString() == getPassword())
    {
            QString date_birth = query.value(11).toString();
            QDate date = QDate::fromString(date_birth,"yyyy-MM-dd");
            date_birth = date.toString("dd-MM-yyyy");
            ui->label_name->setText("Họ tên : " + query.value(1).toString());
            ui->label_email->setText("Email : " + query.value(4).toString());
            ui->label_sex->setText("Giới tính : " + query.value(5).toString());
            ui->label_phonenumber->setText("Số Điện Thoại : " + query.value(6).toString());
            ui->label_datebirth->setText("Ngày Sinh : " + date_birth);
            ui->label_cmnd->setText("Căn Cước Công Dân : " + query.value(10).toString());
            ui->label_address->setText("Địa Chỉ : " + query.value(7).toString());
            ui->label_mssv->setText("Mã Sinh Viên : " + query.value(0).toString());;
            ui->label_major->setText("Ngành : " + query.value(3).toString());
            if(!query.value(12).toString().isEmpty()){ui->label_ethnic->setText("Dân Tộc : " + query.value(12).toString());}
            if(!query.value(13).toString().isEmpty()){ui->label_placeofbirth->setText("Nơi Sinh : " + query.value(13).toString());}
            ui->label_className->setText("Lớp : "+query.value(2).toString());
            ui->label_26->setText("Khoa : "+qr.value(0).toString());
    }
    setPictureForUser();
}



// init
void Student_Interactive_Interface::Init()
{
    QSqlQuery getStudentId;
    QString MSSV,SEX;
    QSqlQuery query,qr;
    query.prepare("SELECT * FROM Student_DATABASE WHERE USERNAME = :username AND PASSWORD = :password");
    query.bindValue(0,getUsername());
    query.bindValue(1,getPassword());
    query.exec();
    query.first();
    qr.prepare("SELECT _thuocKhoa.tenKhoa FROM _thuocKhoa WHERE _thuocKhoa.tenLop=:tenLop");
    qr.bindValue(0,query.value(2).toString());
    qr.exec();
    qr.first();
    if (query.value(8).toString() == getUsername() && query.value(9).toString() == getPassword())
    {
            QString date_birth = query.value(11).toString();
            QDate date = QDate::fromString(date_birth,"yyyy-MM-dd");
            date_birth = date.toString("dd-MM-yyyy");
            ui->label_name->setText("Họ tên : " + query.value(1).toString());
            ui->label_email->setText("Email : " + query.value(4).toString());
            ui->label_sex->setText("Giới tính : " + query.value(5).toString());
            ui->label_phonenumber->setText("Số Điện Thoại : " + query.value(6).toString());
            ui->label_datebirth->setText("Ngày Sinh : " + date_birth);
            ui->label_cmnd->setText("Căn Cước Công Dân : " + query.value(10).toString());
            ui->label_mssv->setText("Mã Sinh Viên : " + query.value(0).toString());;
            ui->label_major->setText("Ngành : " + query.value(3).toString());
            ui->label_address->setText("Địa Chỉ : " + query.value(7).toString());
            if(!query.value(12).toString().isEmpty()){ui->label_ethnic->setText("Dân Tộc : " + query.value(12).toString());}
            if(!query.value(13).toString().isEmpty()){ui->label_placeofbirth->setText("Nơi Sinh : " + query.value(13).toString());}
            ui->label_className->setText("Lớp : "+query.value(2).toString());
            ui->label_26->setText("Khoa : "+qr.value(0).toString());
    }
    ui->tabWidget->setCurrentIndex(0);
    getStudentId.prepare("SELECT MSSV FROM Student_DATABASE WHERE USERNAME = :USERNAME");
    getStudentId.bindValue(0,getUsername());
    getStudentId.exec();
    getStudentId.first();
    MSSV = getStudentId.value(0).toString();
    updateInformation->setIdd(MSSV);
    semesterChange(QDate::currentDate().toString("yyyy"));
    pullOfData();
    ui->tableView_mark->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_registerCourse->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_registeredCourse->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_registeredCourse->verticalHeader()->hide();
}

void Student_Interactive_Interface::showComboBoxUpYear()
{
    ui->comboBox_yearSch->showNormal();
}

void Student_Interactive_Interface::showComboBoxUpSubject()
{
    ui->comboBox_subject->showNormal();
}

//do somthing when pushButton_xem clicked()
void Student_Interactive_Interface::on_pushButton_xem_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab_study);
    execQuery();
}


void Student_Interactive_Interface::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void Student_Interactive_Interface::on_pushButton_information_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab_infor);
}


void Student_Interactive_Interface::on_pushButton_mark_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab_study);
}


void Student_Interactive_Interface::tabSelected()
{
    if(ui->tableView_information->isVisible())
    {
        ui->label_namhoc->hide();
        ui->label_monhoc->hide();
        ui->comboBox_subject->hide();
        ui->comboBox_yearSch->hide();
        ui->comboBox_options->hide();
        ui->pushButton_xem->hide();
        ui->label_9->hide();
        ui->label_10->hide();
        ui->label_11->hide();
        ui->horizontalFrame->hide();
        ui->verticalSpacer_11->changeSize(20,530);
        ui->spacerI->changeSize(20,75);
    }

    if(ui->tableView_mark->isVisible())
    {
        ui->verticalSpacer_11->changeSize(20,170);
        ui->label_namhoc->show();
        ui->label_monhoc->show();
        ui->comboBox_subject->show();
        ui->comboBox_yearSch->show();
        ui->comboBox_options->show();
        ui->pushButton_xem->show();
        ui->label_9->show();
        ui->label_10->show();
        ui->label_11->show();
        ui->spacerI->changeSize(20,30);
        ui->horizontalFrame->show();
    }

    if (ui->tab_learnSchedule->isVisible())
    {
        ui->label_namhoc->show();
        ui->label_monhoc->show();
        ui->comboBox_subject->show();
        ui->comboBox_yearSch->show();
        ui->comboBox_options->show();
        ui->pushButton_xem->show();
        ui->label_9->show();
        ui->label_10->show();
        ui->label_11->show();
        ui->spacerI->changeSize(20,30);
        ui->horizontalFrame->hide();
        ui->verticalSpacer_11->changeSize(20,530);
    }

    if (ui->tab_registerCourse_->isVisible())
    {
        ui->label_namhoc->show();
        ui->label_monhoc->show();
        ui->comboBox_subject->show();
        ui->comboBox_yearSch->show();
        ui->comboBox_options->show();
        ui->pushButton_xem->show();
        ui->label_9->show();
        ui->label_10->show();
        ui->label_11->show();
        ui->spacerI->changeSize(20,30);
        ui->horizontalFrame->hide();
        ui->verticalSpacer_11->changeSize(20,530);
    }
}



bool Student_Interactive_Interface::checkUpdateFormIsVisible()
{
    if (updateInformation->isVisible())
    {
        return true;
    }
    return false;
}



void Student_Interactive_Interface::onSelectionChanged()
{
    QString x = calendar->selectedDate().toString("dd-MM-yyyy");
    QDate day;
    day = QDate::fromString(x,"dd-MM-yyyy");
    clearTimeTable();
    inittabletime(day);
    calendar->close();
}

void Student_Interactive_Interface::on_pushButton_updateInfor_clicked()
{
    updateInformation->close();
    updateInformation->getUserPass(getUsername(),getPassword());
    updateInformation->setDistinguish(1);
    updateInformation->showNormal();
}


void Student_Interactive_Interface::on_pushButton_2_clicked()
{
    QString d2 = ui->label_day2->text();
    QDate date = QDate::fromString(d2,"dd-MM-yyyy");
    date = date.addDays(7);
    ui->label_day2->setText(date.toString("dd-MM-yyyy"));

    QString d3 = ui->label_day3->text();
    date = QDate::fromString(d3,"dd-MM-yyyy");
    date = date.addDays(7);
    ui->label_day3->setText(date.toString("dd-MM-yyyy"));

    QString d4 = ui->label_day4->text();
    date = QDate::fromString(d4,"dd-MM-yyyy");
    d4 = date.toString("yyyy-MM-dd");
    date = date.addDays(7);
    ui->label_day4->setText(date.toString("dd-MM-yyyy"));

    QString d5 = ui->label_day5->text();
    date = QDate::fromString(d5,"dd-MM-yyyy");
    d5 = date.toString("yyyy-MM-dd");
    date = date.addDays(7);
    ui->label_day5->setText(date.toString("dd-MM-yyyy"));

    QString d6 = ui->label_day6->text();
    date = QDate::fromString(d6,"dd-MM-yyyy");
    d6 = date.toString("yyyy-MM-dd");
    date = date.addDays(7);
    ui->label_day6->setText(date.toString("dd-MM-yyyy"));

    QString d7 = ui->label_day7->text();
    date = QDate::fromString(d7,"dd-MM-yyyy");
    d7 = date.toString("yyyy-MM-dd");
    date = date.addDays(7);
    ui->label_day7->setText(date.toString("dd-MM-yyyy"));

    QString d8 = ui->label_day8->text();
    date = QDate::fromString(d8,"dd-MM-yyyy");
    d8 = date.toString("yyyy-MM-dd");
    date = date.addDays(7); 
    ui->label_day8->setText(date.toString("dd-MM-yyyy"));
    clearTimeTable();
    pullOfData();
}

void Student_Interactive_Interface::on_pushButton_clicked()
{
    QString d2 = ui->label_day2->text();
    QDate date = QDate::fromString(d2,"dd-MM-yyyy");
    d2 = date.toString("yyyy-MM-dd");
    date = date.addDays(-7);
    ui->label_day2->setText(date.toString("dd-MM-yyyy"));

    QString d3 = ui->label_day3->text();
    date = QDate::fromString(d3,"dd-MM-yyyy");
    d3 = date.toString("yyyy-MM-dd");
    date = date.addDays(-7);
    ui->label_day3->setText(date.toString("dd-MM-yyyy"));

    QString d4 = ui->label_day4->text();
    date = QDate::fromString(d4,"dd-MM-yyyy");
    d4 = date.toString("yyyy-MM-dd");
    date = date.addDays(-7);
    ui->label_day4->setText(date.toString("dd-MM-yyyy"));

    QString d5 = ui->label_day5->text();
    date = QDate::fromString(d5,"dd-MM-yyyy");
    d5 = date.toString("yyyy-MM-dd");
    date = date.addDays(-7);
    ui->label_day5->setText(date.toString("dd-MM-yyyy"));

    QString d6 = ui->label_day6->text();
    date = QDate::fromString(d6,"dd-MM-yyyy");
    d6 = date.toString("yyyy-MM-dd");
    date = date.addDays(-7);
    ui->label_day6->setText(date.toString("dd-MM-yyyy"));

    QString d7 = ui->label_day7->text();
    date = QDate::fromString(d7,"dd-MM-yyyy");
    d7 = date.toString("yyyy-MM-dd");
    date = date.addDays(-7);
    ui->label_day7->setText(date.toString("dd-MM-yyyy"));

    QString d8 = ui->label_day8->text();
    date = QDate::fromString(d8,"dd-MM-yyyy");
    d8 = date.toString("yyyy-MM-dd");
    date = date.addDays(-7);
    ui->label_day8->setText(date.toString("dd-MM-yyyy"));
    clearTimeTable();
    pullOfData();
}

void Student_Interactive_Interface::on_pushButton_3_clicked()
{
    inittabletime(QDate::currentDate());
}


void Student_Interactive_Interface::pullOfData()
{
    clearTimeTable();
    QSqlQuery query;
    QDate date;
    qDebug()<<"getSTuID "<<getStuId();
    if (index == 1)
    {
        query.prepare("SELECT detailTIMETABLE.courseCode,subjectID,TENMONHOC,dateStudy,studyTime,classroom FROM detailTIMETABLE,registered_course ,SUBJECT_ WHERE detailTIMETABLE.courseCode = registered_course.courseCode AND registered_course.subjectID = SUBJECT_.MAMONHOC AND registered_course.studentID ='"+getStuId()+"' ORDER BY dateStudy ASC");
        qDebug()<<"aaaaaaaaaaa1";
    }
    if (index == 2)
    {
        query.prepare("SELECT testSCHEDULE.courseCode,subjectID,TENMONHOC,testSCHEDULE.dateTest,timeTest,classroom FROM testSCHEDULE,registered_course ,SUBJECT_ WHERE testSCHEDULE.courseCode = registered_course.courseCode AND registered_course.subjectID = SUBJECT_.MAMONHOC AND registered_course.studentID ='"+getStuId()+"' ORDER BY testSCHEDULE.dateTest ASC");
        qDebug()<<"aaaaaaaaaaa2";
    }
    query.exec();
    while (query.next())
    {
        //monday
        date = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");
        if (date.toString("dd-MM-yyyy")==ui->label_day2->text() && query.value(4).toString() == "Sáng")
        {
            ui->label_m2->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_m2->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_m2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_m2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day2->text() && query.value(4).toString() == "Trưa")
        {
            ui->label_n2->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_n2->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_n2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_n2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }

        }
        if (date.toString("dd-MM-yyyy")==ui->label_day2->text() && query.value(4).toString() == "Tối")
        {
            ui->label_ni2->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_ni2->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_ni2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_ni2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        //tuesday
        date = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");
        if (date.toString("dd-MM-yyyy")==ui->label_day3->text() && query.value(4).toString() == "Sáng")
        {
            ui->label_m3->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_m3->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_m3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_m3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day3->text() && query.value(4).toString() == "Trưa")
        {
            ui->label_n3->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_n3->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_n3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_n3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day3->text() && query.value(4).toString() == "Tối")
        {
            ui->label_ni3->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_ni3->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_ni3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_ni3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        //wednesday
        qDebug()<<"DATE STUDY ->"<<query.value(3).toString()<<" "<<query.value(4).toString()<<" "<<query.value(1).toString()<<" "<<query.value(5).toString();

        date = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");

        qDebug()<<"TMMMM-> "<<date.toString("dd-MM-yyyy")<<" "<<ui->label_day4->text();
        if (date.toString("dd-MM-yyyy")==ui->label_day4->text()){qDebug()<<"True";}else{qDebug()<<"False";}
        if(query.value(4).toString() == "Sáng"){qDebug()<<"STRUE";}else{qDebug()<<"SFALSE";}

        if (date.toString("dd-MM-yyyy")==ui->label_day4->text() && query.value(4).toString() == "Sáng")
        {
            ui->label_m4->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_m4->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_m4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_m4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day4->text() && query.value(4).toString() == "Trưa")
        {
            ui->label_n4->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_n4->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_n4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_n4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day4->text() && query.value(4).toString() == "Tối")
        {
            ui->label_ni4->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_ni4->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_ni4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_ni4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        //thursday
        date = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");
        if (date.toString("dd-MM-yyyy")==ui->label_day5->text() && query.value(4).toString() == "Sáng")
        {
            ui->label_m5->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_m5->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_m5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_m5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day5->text() && query.value(4).toString() == "Trưa")
        {
            ui->label_n5->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_n5->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_n5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_n5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day5->text() && query.value(4).toString() == "Tối")
        {
            ui->label_ni5->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_ni5->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_ni5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_ni5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }

        //friday
        date = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");
        if (date.toString("dd-MM-yyyy")==ui->label_day6->text() && query.value(4).toString() == "Sáng")
        {
            ui->label_m6->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_m6->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_m6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_m6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day6->text() && query.value(4).toString() == "Trưa")
        {
            ui->label_n6->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_n6->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_n6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_n6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day6->text() && query.value(4).toString() == "Tối")
        {
            ui->label_ni6->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_ni6->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_ni6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_ni6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }

        //saturday
        date = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");
        if (date.toString("dd-MM-yyyy")==ui->label_day7->text() && query.value(4).toString() == "Sáng")
        {
            ui->label_m7->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_m7->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_m7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_m7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day7->text() && query.value(4).toString() == "Trưa")
        {
            ui->label_n7->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_n7->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_n7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_n7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day7->text() && query.value(4).toString() == "Tối")
        {
            ui->label_ni7->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_ni7->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_ni7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_ni7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }

        //sunday
        date = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");
        if (date.toString("dd-MM-yyyy")==ui->label_day8->text() && query.value(4).toString() == "Sáng")
        {
            ui->label_m8->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_m8->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_m8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_m8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day8->text() && query.value(4).toString() == "Trưa")
        {
            ui->label_n8->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_n8->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_n8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_n8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
        if (date.toString("dd-MM-yyyy")==ui->label_day8->text() && query.value(4).toString() == "Tối")
        {
            ui->label_ni8->setText("S : " + query.value(1).toString()+"\nP : "+query.value(5).toString());
            ui->label_ni8->setToolTip(query.value(2).toString());
            if (index == 1)
            {
                ui->label_ni8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
            }
            if (index == 2)
            {
                ui->label_ni8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
            }
        }
    }
}

void Student_Interactive_Interface::clearTimeTable()
{
    ui->label_m2->clear();
    ui->label_n2->clear();
    ui->label_ni2->clear();
    ui->label_m2->setToolTip("");
    ui->label_n2->setToolTip("");
    ui->label_ni2->setToolTip("");
    ui->label_m2->setStyleSheet("none");
    ui->label_n2->setStyleSheet("none");
    ui->label_ni2->setStyleSheet("none");

    ui->label_m3->clear();
    ui->label_n3->clear();
    ui->label_ni3->clear();
    ui->label_m3->setToolTip("");
    ui->label_n3->setToolTip("");
    ui->label_ni3->setToolTip("");
    ui->label_m3->setStyleSheet("none");
    ui->label_n3->setStyleSheet("none");
    ui->label_ni3->setStyleSheet("none");

    ui->label_m4->clear();
    ui->label_n4->clear();
    ui->label_ni4->clear();
    ui->label_m4->setToolTip("");
    ui->label_n4->setToolTip("");
    ui->label_ni4->setToolTip("");
    ui->label_m4->setStyleSheet("none");
    ui->label_n4->setStyleSheet("none");
    ui->label_ni4->setStyleSheet("none");

    ui->label_m5->clear();
    ui->label_n5->clear();
    ui->label_ni5->clear();
    ui->label_m5->setToolTip("");
    ui->label_n5->setToolTip("");
    ui->label_ni5->setToolTip("");
    ui->label_m5->setStyleSheet("none");
    ui->label_n5->setStyleSheet("none");
    ui->label_ni5->setStyleSheet("none");

    ui->label_m6->clear();
    ui->label_n6->clear();
    ui->label_ni6->clear();
    ui->label_m6->setToolTip("");
    ui->label_n6->setToolTip("");
    ui->label_ni6->setToolTip("");
    ui->label_m6->setStyleSheet("none");
    ui->label_n6->setStyleSheet("none");
    ui->label_ni6->setStyleSheet("none");

    ui->label_m7->clear();
    ui->label_n7->clear();
    ui->label_ni7->clear();
    ui->label_m7->setToolTip("");
    ui->label_n7->setToolTip("");
    ui->label_ni7->setToolTip("");
    ui->label_m7->setStyleSheet("none");
    ui->label_n7->setStyleSheet("none");
    ui->label_ni7->setStyleSheet("none");

    ui->label_m8->clear();
    ui->label_n8->clear();
    ui->label_ni8->clear();
    ui->label_m8->setToolTip("");
    ui->label_n8->setToolTip("");
    ui->label_ni8->setToolTip("");
    ui->label_m8->setStyleSheet("none");
    ui->label_n8->setStyleSheet("none");
    ui->label_ni8->setStyleSheet("none");
}

void Student_Interactive_Interface::getIndex(int index)
{
    this->index = index;
}

void Student_Interactive_Interface::on_comboBox_options_activated(int index)
{
    if (index == 1)
    {
        QPixmap pixmap(":/icon-file/schedule_icon.png");
        QIcon icon;
        icon.addPixmap(pixmap);
        ui->tabWidget->insertTab(3,ui->tab_learnSchedule,icon,"Lịch Học");
        ui->tabWidget->setCurrentWidget(ui->tab_learnSchedule);
        ui->pushButton_5->setText("In lịch học");
        ui->pushButton_5->setIcon(QIcon(":/icon-file/schedule_icon.png"));
        getIndex(1);
        inittabletime(QDate::currentDate());
    }
    if (index == 2)
    {
        QPixmap pixmap(":/icon-file/schedule_icon.png");
        QIcon icon;
        icon.addPixmap(pixmap);
        ui->tabWidget->insertTab(3,ui->tab_learnSchedule,icon,"Lịch Thi");
        ui->tabWidget->setCurrentWidget(ui->tab_learnSchedule);
        ui->pushButton_5->setText("In lịch thi");
        ui->pushButton_5->setIcon(QIcon(":/icon-file/test_calendar.png"));
        getIndex(2);
        inittabletime(QDate::currentDate());
    }
    if (index == 3)
    {
        getIndex(0);
        ui->tabWidget->setCurrentWidget(ui->tab_registerCourse_);
    }

    if (index == 4)
    {
        getIndex(0);
        QPixmap pixmap(":/icon-file/debt_student_standard.png");
        QIcon icon(pixmap);
        wid->close();
        updateDebtStudent();
        wid->show();
    }
}



void Student_Interactive_Interface::inittabletime(QDate date_)
{
    QDate date;
    date = date_;
    int n = date.dayOfWeek();
    switch (n)
    {
    case 1:
        //Thứ 2
        ui->label_day2->setText(date.toString("dd-MM-yyyy"));
        ui->label_day3->setText(date.addDays(1).toString("dd-MM-yyyy"));
        ui->label_day4->setText(date.addDays(2).toString("dd-MM-yyyy"));
        ui->label_day5->setText(date.addDays(3).toString("dd-MM-yyyy"));
        ui->label_day6->setText(date.addDays(4).toString("dd-MM-yyyy"));
        ui->label_day7->setText(date.addDays(5).toString("dd-MM-yyyy"));
        ui->label_day8->setText(date.addDays(6).toString("dd-MM-yyyy"));
        break;
    case 2:
        //Thứ 3
        ui->label_day2->setText(date.addDays(-1).toString("dd-MM-yyyy"));
        ui->label_day3->setText(date.toString("dd-MM-yyyy"));
        ui->label_day4->setText(date.addDays(1).toString("dd-MM-yyyy"));
        ui->label_day5->setText(date.addDays(2).toString("dd-MM-yyyy"));
        ui->label_day6->setText(date.addDays(3).toString("dd-MM-yyyy"));
        ui->label_day7->setText(date.addDays(4).toString("dd-MM-yyyy"));
        ui->label_day8->setText(date.addDays(5).toString("dd-MM-yyyy"));
        break;
    case 3:
        //Thứ 4
        ui->label_day2->setText(date.addDays(-2).toString("dd-MM-yyyy"));
        ui->label_day3->setText(date.addDays(-1).toString("dd-MM-yyyy"));
        ui->label_day4->setText(date.toString("dd-MM-yyyy"));
        ui->label_day5->setText(date.addDays(1).toString("dd-MM-yyyy"));
        ui->label_day6->setText(date.addDays(2).toString("dd-MM-yyyy"));
        ui->label_day7->setText(date.addDays(3).toString("dd-MM-yyyy"));
        ui->label_day8->setText(date.addDays(4).toString("dd-MM-yyyy"));
        break;
    case 4:
        //Thứ 5
        ui->label_day2->setText(date.addDays(-3).toString("dd-MM-yyyy"));
        ui->label_day3->setText(date.addDays(-2).toString("dd-MM-yyyy"));
        ui->label_day4->setText(date.addDays(-1).toString("dd-MM-yyyy"));
        ui->label_day5->setText(date.toString("dd-MM-yyyy"));
        ui->label_day6->setText(date.addDays(1).toString("dd-MM-yyyy"));
        ui->label_day7->setText(date.addDays(2).toString("dd-MM-yyyy"));
        ui->label_day8->setText(date.addDays(3).toString("dd-MM-yyyy"));
        break;
    case 5:
        //Thứ 6
        ui->label_day2->setText(date.addDays(-4).toString("dd-MM-yyyy"));
        ui->label_day3->setText(date.addDays(-3).toString("dd-MM-yyyy"));
        ui->label_day4->setText(date.addDays(-2).toString("dd-MM-yyyy"));
        ui->label_day5->setText(date.addDays(-1).toString("dd-MM-yyyy"));
        ui->label_day6->setText(date.toString("dd-MM-yyyy"));
        ui->label_day7->setText(date.addDays(1).toString("dd-MM-yyyy"));
        ui->label_day8->setText(date.addDays(2).toString("dd-MM-yyyy"));
        break;
    case 6:
        //Thứ 7
        ui->label_day2->setText(date.addDays(-5).toString("dd-MM-yyyy"));
        ui->label_day3->setText(date.addDays(-4).toString("dd-MM-yyyy"));
        ui->label_day4->setText(date.addDays(-3).toString("dd-MM-yyyy"));
        ui->label_day5->setText(date.addDays(-2).toString("dd-MM-yyyy"));
        ui->label_day6->setText(date.addDays(-1).toString("dd-MM-yyyy"));
        ui->label_day7->setText(date.toString("dd-MM-yyyy"));
        ui->label_day8->setText(date.addDays(1).toString("dd-MM-yyyy"));
        break;
    case 7:
        //Chủ Nhật
        ui->label_day2->setText(date.addDays(-6).toString("dd-MM-yyyy"));
        ui->label_day3->setText(date.addDays(-5).toString("dd-MM-yyyy"));
        ui->label_day4->setText(date.addDays(-4).toString("dd-MM-yyyy"));
        ui->label_day5->setText(date.addDays(-3).toString("dd-MM-yyyy"));
        ui->label_day6->setText(date.addDays(-2).toString("dd-MM-yyyy"));
        ui->label_day7->setText(date.addDays(-1).toString("dd-MM-yyyy"));
        ui->label_day8->setText(date.toString("dd-MM-yyyy"));
        break;
    }
    pullOfData();
}

void Student_Interactive_Interface::on_pushButton_4_clicked()
{
    calendar->close();
    QFont font;
    QTextCharFormat format;
    format.setForeground(QBrush("black"));
    format.setBackground(QBrush("#93dcdc"));
    calendar->setHeaderTextFormat(format);
    calendar->setWindowTitle("Xem lịch");
    QPixmap pixmap(":/icon-file/schedule_icon.png");
    QIcon icon(pixmap);
    calendar->setWindowIcon(icon);
    font.setFamily("Courier");
    calendar->setStyleSheet("QCalendarWidget{background-color : rgb(255, 255, 206);color : black;}"
                            "QCalendarWidget QToolButton{width:70;background-color:rgb(255, 170, 0);color:black;border-radius:4px;}"
                            "QCalendarWidget QToolButton:pressed{background-color:rgb(255, 170, 0);color:black;border : 2px solid rgb(68, 68, 68);}"
                            "QCalendarWidget QToolButton:hover{background-color:rgb(255, 170, 0);color:black;border : 2px solid rgb(68, 68, 68);}"
                            "QCalendarWidget QSpinBox::up-button{subcontrol-origin: border;subcontrol-position: top right;  width:20px; }"
                            "QCalendarWidget QSpinBox::down-button{subcontrol-origin: border;subcontrol-position: bottom right;  width:20px;}"
                            "QCalendarWidget QSpinBox::up-arrow{width:10px;height:10px; }"
                            "QCalendarWidget QSpinBox::down-arrow{width:10px;height:10px;}"
                            "QCalendarWidget QMenu{background-color:white;selection-background-color:rgb(255, 170, 0);color:black;selection-color:black;}"
                            "QCalendarWidget QAbstractItemView:enabled{selection-background-color:rgb(255, 170, 0);outline:0px;color:black;}");
    calendar->setFixedSize(400,350);
    calendar->setGridVisible(true);
    calendar->setFont(font);
    calendar->setNavigationBarVisible(true);
    calendar->setContentsMargins(10,10,10,10);
    calendar->show();
}


void Student_Interactive_Interface::on_tableView_registerCourse_clicked(const QModelIndex &index)
{
    indexForTableResB = -1;
    getIndexTableViewResA(index.row());
}

void Student_Interactive_Interface::on_pushButton_regis_clicked()
{
    QSqlQuery insertQuery,addDebt,temp,up,debtt;indexForTableResB = -1;ui->tableView_registeredCourse->clearSelection();
    QString _SUBJECTNAME,MSSV,teacherID;int numre;
    QString semesterR = ui->comboBox->currentText();
    qDebug()<<semesterR;
    ui->tableView_registeredCourse->horizontalHeader()->show();
    int indeex = this->indexForTableResA;
    QString courseCode = ui->tableView_registerCourse->model()->index(indexForTableResA,0,QModelIndex()).data().toString();
    QString subjectName = ui->tableView_registerCourse->model()->index(indexForTableResA,1,QModelIndex()).data().toString();
    QDate dateStart = ui->tableView_registerCourse->model()->index(indexForTableResA,2,QModelIndex()).data().toDate();
    QDate dateEnd = ui->tableView_registerCourse->model()->index(indexForTableResA,3,QModelIndex()).data().toDate();
    int num = ui->tableView_registerCourse->model()->index(indexForTableResA,6,QModelIndex()).data().toInt();
    QString nameTeacher = ui->tableView_registerCourse->model()->index(indexForTableResA,7,QModelIndex()).data().toString();
    teacherID = ui->tableView_registerCourse->model()->index(indexForTableResA,8,QModelIndex()).data().toString();
    numre = ui->tableView_registerCourse->model()->index(indexForTableResA,9,QModelIndex()).data().toInt();
    qDebug()<<"TRUE FALSE "<<courseCode.isNull()<<" "<<subjectName.isNull()<<""<<dateStart.isNull()<<" "<<dateEnd.isNull()<<" "<<nameTeacher.isEmpty()<<" "<<teacherID.isEmpty() <<" "<< num ;
    QSqlQuery qr;
    addDebt.prepare("INSERT INTO debtOfStudent(courseCode,subjectName,debt,numOfRes,debtTotal,studentID,semester)"
                    "VALUES(:course,:subjectName,:debt,:numOfRes,:debtTotal,:stuID,:semester)");
    if (!courseCode.isNull() && !subjectName.isNull() && !dateStart.isNull() && !dateEnd.isNull()&& !nameTeacher.isEmpty()&& !teacherID.isEmpty() && num < 70)
    {
        ui->tableView_registerCourse->hideRow(indexForTableResA);
        this->indexForTableResA = -1;
        qr.prepare("INSERT INTO tempTABLE(courseCode,subjectName,dateStart,dateEnd,nameOfTeacher,numRes) VALUES(:courseCode,:subjectname,:dateStart,:dateEnd,:nameTeacher,:numRes)");
        qr.bindValue(0,courseCode);
        qr.bindValue(1,subjectName);
        qr.bindValue(2,dateStart.toString("yyyy-MM-dd"));
        qr.bindValue(3,dateEnd.toString("yyyy-MM-dd"));
        qr.bindValue(4,nameTeacher);
        qr.bindValue(5,numre);
        qr.exec();
        qDebug()<<"MMM "<<courseCode<<" "<<subjectName<<" "<<dateStart.toString("yyyy-MM-dd")<<" "<<dateEnd.toString("yyyy-MM-dd")<<" "<<nameTeacher;
        tableModelRes->select();
    }
    if (!checkDuplicateObject(courseCode))
    {
        qOObject.getAll(indeex,courseCode,subjectName,dateStart,dateEnd);
        vec.push_back(qOObject);
    }
    if(num == 70){messageBoxOneButton("Thông báo !","Đăng kí thất bại. Số lượng sinh viên đã đủ !","Đồng ý",350,250,85,28);};
    if (!courseCode.isNull() && !subjectName.isNull() && !dateStart.isNull() && !dateEnd.isNull() && num < 70)
    {
        qDebug()<<"aaaaaaaaaaa3";
        temp.prepare("SELECT SUBJECT_.MAMONHOC FROM SUBJECT_ WHERE SUBJECT_.TENMONHOC = :SUBJECTNAME");
        temp.bindValue(0,subjectName);
        temp.exec();
        temp.first();
        _SUBJECTNAME = temp.value(0).toString();
        insertQuery.prepare("INSERT INTO registered_course(courseCode,subjectID,dateStartStudy,dateEndStudy,semester,studentID,nameOfTeacher,TeacherId,numRes) VALUES(:COURSEID,:SUBJECTNAME,:DATESTART,:DATEEND,:SEMESTER,:MSSV,:nameTeacher,:id,:numRes)");
        insertQuery.bindValue(0,courseCode);
        insertQuery.bindValue(1,_SUBJECTNAME);
        insertQuery.bindValue(2,dateStart.toString("yyyy-MM-dd"));
        insertQuery.bindValue(3,dateEnd.toString("yyyy-MM-dd"));
        insertQuery.bindValue(4,semesterR);
        insertQuery.bindValue(5,getStuId());
        insertQuery.bindValue(6,nameTeacher);
        insertQuery.bindValue(7,teacherID);
        insertQuery.bindValue(8,numre);
        insertQuery.exec();

        debtt.prepare("SELECT debt FROM DEBT WHERE subjectName = '"+_SUBJECTNAME+"'");
        debtt.exec();debtt.first();
        up.prepare("EXEC _updateResQuantity '"+courseCode+"', '"+semesterR+"', 1 ");
        addDebt.bindValue(0,courseCode);
        addDebt.bindValue(1,subjectName);
        addDebt.bindValue(2,debtt.value(0).toInt());
        addDebt.bindValue(3,numre);
        addDebt.bindValue(4,numre*debtt.value(0).toInt());
        addDebt.bindValue(5,getStuId());
        addDebt.bindValue(6,semesterR);
        addDebt.exec();
        if(up.exec() && num < 70){messageBoxOneButton("Thông báo !","Đăng kí môn thành công !","Đồng ý",250,150,80,27);}
    }
    pullOfData();
    tableModelDebt->select();
}


void Student_Interactive_Interface::on_pushButton_cancelRegis_clicked()
{
    QSqlQuery getStudentId,up,deleteDebt,del;indexForTableResA=-1;
    QString MSSV,idT;
    QString courseID = ui->tableView_registeredCourse->model()->index(indexForTableResB,0,QModelIndex()).data().toString();
    if(!courseID.isEmpty()){
        up.prepare("EXEC _updateResQuantity '"+courseID+"', '"+ui->comboBox->currentText()+"', 2 ");
        if(up.exec()){messageBoxOneButton("Thông báo !","Hủy đăng kí thành công !","Đồng ý",250,150,80,27);};//else{messageBoxOneButton("Thông báo !","Đăng kí thất bại. Số lượng sinh viên đã đủ !","Đồng ý",350,250,85,28);};
        ui->tableView_registerCourse->showRow(returnIndex(indexForTableResB));
    }
    del.prepare("DELETE FROM tempTABLE WHERE tempTABLE.courseCode = '"+courseID+"'");
    del.exec();
    del.prepare("DELETE FROM registered_course WHERE registered_course.courseCode = :COURSEID AND  registered_course.studentID= :MSSV");
    del.bindValue(0,courseID);
    del.bindValue(1,getStuId());
    del.exec();
    deleteDebt.prepare("DELETE FROM debtOfStudent WHERE courseCode='"+courseID+"' AND studentID='"+getStuId()+"'");
    deleteDebt.exec();
    this->indexForTableResB = -1;
    tableModelRes->select();
    pullOfData();
    tableModelDebt->select();
    semesterChange(ui->comboBox->currentText());
}

int Student_Interactive_Interface::returnIndex(int x)
{
    QString courseCODE = ui->tableView_registeredCourse->model()->index(x,0,QModelIndex()).data().toString();
    QString nameCourse = ui->tableView_registeredCourse->model()->index(x,1,QModelIndex()).data().toString();
    for (auto i = vec.begin();i != vec.end() ; i++)
    {
        if (i->getCourseCode() == courseCODE && i->getSubjectName() == nameCourse)
        {
            return i->getIndexOfObject();
        }
    }
    return -1;
}

void Student_Interactive_Interface::on_tableView_registeredCourse_clicked(const QModelIndex &index)
{
    indexForTableResA = -1;
    getIndexTableViewResB(index.row());
}

bool Student_Interactive_Interface::checkDuplicateObject(QString coursecode)
{
    for (auto i = vec.begin();i != vec.end() ;i++ )
    {
        if (i->getCourseCode() == coursecode)
        {
            return true;
        }
    }
    return false;
}


QString Student_Interactive_Interface::getString(QString stringg)
{
    return stringg;
}

bool Student_Interactive_Interface::checkTrueExistForEveryRow()
{
    QSqlQuery queryForMainTable,insertQuery,getStudentId,queryForTempTable,temp;
    queryForMainTable.prepare("SELECT * FROM registered_course");
    queryForTempTable.prepare("SELECT * FROM tempTABLE");
    queryForTempTable.exec();
    QString COURSEID,SUBJECTNAME,COURSEID_,SUBJECTID,_SUBJECTNAME,MSSV;
    QDate DATESTART,DATEEND,DATESTART_,DATEEND_;
    QString errorr ;
    while (queryForTempTable.next())
    {
        COURSEID = queryForTempTable.value(0).toString();
        SUBJECTNAME = queryForTempTable.value(1).toString();
        DATESTART = queryForTempTable.value(2).toDate();
        DATEEND = queryForTempTable.value(3).toDate();
        queryForMainTable.exec();
        while (queryForMainTable.next())
        {
            COURSEID_ = queryForMainTable.value(0).toString();
            SUBJECTID = queryForMainTable.value(1).toString();
            DATESTART_ = queryForMainTable.value(2).toDate();
            DATEEND_ = queryForMainTable.value(3).toDate();
            temp.prepare("SELECT SUBJECT_.TENMONHOC FROM SUBJECT_ WHERE SUBJECT_.MAMONHOC = :SUBJECTID");
            temp.bindValue(0,SUBJECTID);
            temp.exec();
            temp.first();
            _SUBJECTNAME = temp.value(0).toString();

            if (COURSEID == COURSEID_ && SUBJECTNAME == _SUBJECTNAME && DATESTART == DATESTART_ && DATEEND == DATEEND_)
            {
                errorr.append(_SUBJECTNAME + "\n");
            }
            qDebug()<<"aaaaaaaaaaa4";
        }
    }
    if (errorr.isEmpty())
    {
        return false;
    }
    getText(errorr);
    return true;
}

void Student_Interactive_Interface::getText(QString txt)
{
    this->text = txt;
}


void Student_Interactive_Interface::initAgain(QString year)
{
    QString COURSEID,SUBJECTNAME,subjectiD,cousrse,subjectid,ahihi,MSSV,YEARA,YEARB,nameTeacher,teacherId,idTeacher;
    QDate dateStart_,dateEnd_;int numr,numm;
    QSqlQuery queryForSetupRes,queryForRes,temp,qr,getStudentId;
    queryForSetupRes.prepare("SELECT * FROM registerForTheCourse WHERE semester = :YEAR");
    queryForSetupRes.bindValue(0,year);
    queryForRes.prepare("SELECT * FROM registered_course WHERE studentID = :MSSV AND registered_course.semester = :YEAR");
    queryForRes.bindValue(0,getStuId());
    queryForRes.bindValue(1,year);
    int k =0;
    queryForSetupRes.exec();
    while (queryForSetupRes.next())
    {
        COURSEID = queryForSetupRes.value(0).toString();
        SUBJECTNAME = queryForSetupRes.value(1).toString();
        dateStart_ = queryForSetupRes.value(2).toDate();
        dateEnd_ = queryForSetupRes.value(3).toDate();
        YEARA = queryForSetupRes.value(4).toString();
        teacherId = queryForSetupRes.value(8).toString();
        numr = queryForSetupRes.value(9).toInt();

        temp.prepare("SELECT SUBJECT_.MAMONHOC FROM SUBJECT_ WHERE SUBJECT_.TENMONHOC = :SUBJECTNAME");
        temp.bindValue(0,SUBJECTNAME);
        temp.exec();
        temp.first();
        subjectiD = temp.value(0).toString();
        queryForRes.exec();
        while (queryForRes.next())
        {
            cousrse = queryForRes.value(0).toString();
            subjectid = queryForRes.value(1).toString();
            YEARB = queryForRes.value(4).toString();
            nameTeacher = queryForRes.value(6).toString();
            idTeacher = queryForRes.value(7).toString();
            numm = queryForRes.value(8).toInt();

            if (COURSEID == cousrse && subjectiD == subjectid && YEARA == YEARB && teacherId == idTeacher)
            {
                temp.prepare("SELECT SUBJECT_.TENMONHOC FROM SUBJECT_ WHERE SUBJECT_.MAMONHOC = :SUBJECTNAME");
                temp.bindValue(0,subjectid);
                temp.exec();
                temp.first();
                ahihi = temp.value(0).toString();
                qOObject.getAll(k,COURSEID,ahihi,dateStart_,dateEnd_);
                vec.push_back(qOObject);
                ui->tableView_registerCourse->hideRow(k);
                qr.prepare("INSERT INTO tempTABLE(courseCode,subjectName,dateStart,dateEnd,nameOfTeacher,numRes) VALUES(:courseCode,:subjectname,:dateStart,:dateEnd,:nameTeacher,:num)");
                qr.bindValue(0,cousrse);
                qr.bindValue(1,ahihi);
                qr.bindValue(2,dateStart_.toString("yyyy-MM-dd"));
                qr.bindValue(3,dateEnd_.toString("yyyy-MM-dd"));
                qr.bindValue(4,nameTeacher);
                qr.bindValue(5,numr);
                qr.exec();
                tableModelRes->select();
            }

        }
        k++;
    }
    inittabletime(QDate::currentDate());
}


void Student_Interactive_Interface::getIndexTableViewResA(int a)
{
    this->indexForTableResA = a;
}

void Student_Interactive_Interface::getIndexTableViewResB(int a)
{
    this->indexForTableResB = a;
}

void Student_Interactive_Interface::updateDebtStudent()
{
    tableVieww->horizontalHeader()->setSectionsClickable(false);
    tableVieww->setWordWrap(true);
    tableVieww->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tableVieww->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tableVieww->horizontalScrollBar()->show();
    tableVieww->verticalScrollBar()->show();
    tableVieww->setFocusPolicy(Qt::NoFocus);
    tableVieww->setStyleSheet("QHeaderView{border:none;border-bottom:1px solid}"
    "QHeaderView::section{border:none;background-color: rgb(240, 255, 255);border-left-color:black;border-left : 1px solid;}"
    "QTableView{background-color: #ecec76; selection-background-color: #d0d09c; selection-color: black;}"
    "QScrollBar::handle:horizontal{boder-radius : 7px;}");
    QSqlQuery getStudentId;
    QLabel* label = new QLabel(this);
    QVBoxLayout* vLayout = new QVBoxLayout();
    QString MSSV;
    getStudentId.prepare("SELECT MSSV FROM Student_DATABASE WHERE USERNAME = :USERNAME");
    getStudentId.bindValue(0,getUsername());
    getStudentId.exec();
    getStudentId.first();
    MSSV = getStudentId.value(0).toString();
    tableModelDebt->setTable("debtOfStudent");
    tableModelDebt->setFilter("studentID = '"+MSSV+"'");
    tableModelDebt->select();
    tableVieww->setModel(tableModelDebt);
    tableVieww->hideColumn(5);
    vLayout->addWidget(label);
    vLayout->addWidget(tableVieww);
    label->setText("Danh sách công nợ sinh viên");
    wid->setLayout(vLayout);
    wid->setFixedWidth(640);
    wid->setFixedHeight(500);
    wid->setWindowTitle("Công nợ sinh viên");
    QPixmap pixmap(":/icon-file/debt_student_standard.png");
    QIcon icon(pixmap);
    wid->setWindowIcon(icon);
    wid->setStyleSheet("font : 8pt Courier");
    tableVieww->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    tableVieww->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableVieww->verticalHeader()->hide();
    tableModelDebt->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    tableModelDebt->setHeaderData(1,Qt::Horizontal,tr("Môn học"));
    tableModelDebt->setHeaderData(2,Qt::Horizontal,tr("Tiền (đv:đồng)"));
    tableModelDebt->setHeaderData(3,Qt::Horizontal,tr("Số tín chỉ"));
    tableModelDebt->setHeaderData(4,Qt::Horizontal,tr("Học phí"));
    tableModelDebt->setHeaderData(6,Qt::Horizontal,tr("Năm học"));
    tableModelDebt->setHeaderData(7,Qt::Horizontal,tr("Trạng thái"));
    tableVieww->setColumnWidth(1,180);
    tableVieww->setColumnWidth(2,150);
    tableVieww->setColumnWidth(4,150);
    tableVieww->setColumnWidth(7,130);
    tableModelDebt->sort(0,Qt::AscendingOrder);
}

void Student_Interactive_Interface::setPictureForUser()
{
    QSqlQuery queryy;
    QSqlQuery getStudentId;
    QString MSSV;
    getStudentId.prepare("SELECT MSSV FROM Student_DATABASE WHERE USERNAME = :USERNAME");
    getStudentId.bindValue(0,getUsername());
    getStudentId.exec();
    getStudentId.first();
    MSSV = getStudentId.value(0).toString();
    queryy.prepare("SELECT * FROM IMAGE_USERS WHERE userID = :MSSV");
    queryy.bindValue(0,MSSV);
    queryy.exec();
    queryy.first();
    QByteArray byte = queryy.value(1).toByteArray();
    if (!byte.isEmpty())
    {
        QPixmap pix(byte);
        pix.scaled(100,150);
        ui->label_pic_personal->setPixmap(pix);
    }
    else
    {
        QSqlQuery qry;
        QString SEX;
        qry.prepare("SELECT SEX FROM Student_DATABASE WHERE USERNAME = :username");
        qry.bindValue(0,getUsername());
        qry.exec();
        qry.first();
        SEX = qry.value(0).toString();
        if ( SEX == "Nam")
        {
            QPixmap pixmap(":/icon-file/boyy_student.png");//:/icon-file/boy_studeent.png
            ui->label_pic_personal->setPixmap(pixmap);
        }
        if (SEX == "Nữ")
        {
            QPixmap pixmap(":/icon-file/girll_student.png");
            ui->label_pic_personal->setPixmap(pixmap);
        }
    }
}


void Student_Interactive_Interface::on_actionTh_ng_tin_sinh_vi_n_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_infor);
}

void Student_Interactive_Interface::on_actionK_t_qua_ho_c_t_p_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_study);
}

void Student_Interactive_Interface::on_actionLi_ch_ho_c_triggered()
{
    ui->tabWidget->insertTab(3,ui->tab_learnSchedule,"Lịch Học");
    ui->tabWidget->setCurrentWidget(ui->tab_learnSchedule);
    ui->pushButton_5->setText("In lịch học");
    ui->pushButton_5->setIcon(QIcon(":/icon-file/schedule_icon.png"));
    ui->comboBox_options->setCurrentIndex(1);
    getIndex(1);
    inittabletime(QDate::currentDate());
}

void Student_Interactive_Interface::on_actionLi_ch_thi_triggered()
{
    ui->tabWidget->insertTab(3,ui->tab_learnSchedule,"Lịch Thi");
    ui->tabWidget->setCurrentWidget(ui->tab_learnSchedule);
    ui->pushButton_5->setText("In lịch thi");
    ui->pushButton_5->setIcon(QIcon(":/icon-file/test_calendar.png"));
    ui->comboBox_options->setCurrentIndex(2);
    getIndex(2);
    inittabletime(QDate::currentDate());
}

void Student_Interactive_Interface::on_action_ng_ki_ho_c_ph_n_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_registerCourse_);
}

void Student_Interactive_Interface::on_actionThoa_t_triggered()
{
    wid->close();
    updateDebtStudent();
    wid->show();
}

void Student_Interactive_Interface::on_actionThoa_t_2_triggered()
{
    this->close();
}


void Student_Interactive_Interface::semesterChange(QString year)
{
    tableModel->clear();
    tableModel->setTable("registerForTheCourse");
    tableModel->setFilter("semester = '"+year+"'");
    tableModel->select();
    ui->tableView_registerCourse->setModel(tableModel);
    ui->tableView_registerCourse->setColumnWidth(0,100);
    ui->tableView_registerCourse->setColumnWidth(1,200);
    ui->tableView_registerCourse->setColumnWidth(2,150);
    ui->tableView_registerCourse->setColumnWidth(3,150);
    ui->tableView_registerCourse->setColumnWidth(4,100);
    ui->tableView_registerCourse->setColumnWidth(5,150);
    ui->tableView_registerCourse->setColumnWidth(6,150);
    ui->tableView_registerCourse->setColumnWidth(7,180);

    ui->tableView_registerCourse->verticalHeader()->hide();
    ui->tableView_registerCourse->hideColumn(4);
    ui->tableView_registerCourse->hideColumn(8);
    ui->tableView_registerCourse->model()->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    ui->tableView_registerCourse->model()->setHeaderData(1,Qt::Horizontal,tr("Môn học"));
    ui->tableView_registerCourse->model()->setHeaderData(2,Qt::Horizontal,tr("Ngày bắt đầu"));
    ui->tableView_registerCourse->model()->setHeaderData(3,Qt::Horizontal,tr("Ngày kết thúc"));
    ui->tableView_registerCourse->model()->setHeaderData(5,Qt::Horizontal,tr("Số lượng Tối đa"));
    ui->tableView_registerCourse->model()->setHeaderData(6,Qt::Horizontal,tr("Số lượng đăng kí"));
    ui->tableView_registerCourse->model()->setHeaderData(7,Qt::Horizontal,tr("Giảng viên"));
    ui->tableView_registerCourse->model()->setHeaderData(9,Qt::Horizontal,tr("Số tín chỉ"));
    QSqlQuery qqry;
    qqry.prepare("TRUNCATE TABLE tempTABLE");
    qqry.exec();
    tableModelRes->setTable("tempTABLE");
    tableModelRes->select();
    ui->tableView_registeredCourse->setModel(tableModelRes);
    ui->tableView_registeredCourse->model()->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    ui->tableView_registeredCourse->model()->setHeaderData(1,Qt::Horizontal,tr("Môn học"));
    ui->tableView_registeredCourse->model()->setHeaderData(2,Qt::Horizontal,tr("Ngày bắt đầu"));
    ui->tableView_registeredCourse->model()->setHeaderData(3,Qt::Horizontal,tr("Ngày kết thúc"));
    ui->tableView_registeredCourse->model()->setHeaderData(4,Qt::Horizontal,tr("Giảng viên"));
    ui->tableView_registeredCourse->model()->setHeaderData(5,Qt::Horizontal,tr("Số tín chỉ"));

    ui->tableView_registeredCourse->setColumnWidth(0,100);
    ui->tableView_registeredCourse->setColumnWidth(1,240);
    ui->tableView_registeredCourse->setColumnWidth(2,150);
    ui->tableView_registeredCourse->setColumnWidth(3,150);
    ui->tableView_registeredCourse->setColumnWidth(4,180);
    showRowChange();
    initAgain(year);
    year.clear();
}


void Student_Interactive_Interface::initComboBox()
{
    ui->comboBox->clear();
    ui->comboBox_yearSch->clear();
    QSqlQuery query,qry,sub,qury;
    QString qstr;
    qury.prepare("SELECT DISTINCT semester FROM registerForTheCourse ORDER BY semester DESC");
    qury.exec();
    qry.prepare("SELECT MSSV FROM Student_DATABASE WHERE USERNAME='"+getUsername()+"'");
    qry.exec();
    qry.first();
    query.prepare("SELECT DISTINCT SCHOOL_YEAR FROM MARK_OF_STUDENTS WHERE MSSV='"+qry.value(0).toString()+"' ORDER BY SCHOOL_YEAR DESC");
    query.bindValue(0,getUsername());
    sub.prepare("SELECT DISTINCT SUBJECT_.TENMONHOC FROM SUBJECT_,MARK_OF_STUDENTS WHERE SUBJECT_.MAMONHOC = MARK_OF_STUDENTS.MAMONHOC AND MARK_OF_STUDENTS.MSSV='"+qry.value(0).toString()+"'");
    query.exec();
    sub.exec();
    ui->comboBox->addItem("");
    ui->comboBox_yearSch->addItem("");
    ui->comboBox_subject->addItem("");
    while (query.next()){ui->comboBox_yearSch->addItem(query.value(0).toString());}
    while(sub.next()){ui->comboBox_subject->addItem(sub.value(0).toString());}
    while(qury.next()){ui->comboBox->addItem(qury.value(0).toString());}
    ui->comboBox_yearSch->setCurrentIndex(1);
}

void Student_Interactive_Interface::on_action_ng_xu_t_triggered()
{
    AutoFillPassWord a;
    a.setUser(getUsername());a.setPass(getPassword());
    if(!getSaveOrNot()){
        bool i = messageBoxTwoButton("Thông báo !","Bạn có muốn lưu mật khẩu cho lần đăng nhập tiếp theo !","Lưu","Không lưu",350,250,80,28);
        checkClose = false;
        if(i){setSaveOrNot(true);a.setBool(getSaveOrNot());vecT.push_back(a);writeToFile();}
        for(unsigned long long i = 0;i < vecT.size();i++)
        {
            qDebug()<<"BOOOB : "<<vecT[i].getBool()<<":"<<vecT[i].getUser()<<":"<<vecT[i].getPass()<<"\n";
        }
        this->close();
    }
    else{this->close();}
}

void Student_Interactive_Interface::UpdateTime()
{
    ui->label_12->setText(QDate::currentDate().toString() +"  "+ QTime::currentTime().toString("hh:mm:ss"));
}



void Student_Interactive_Interface::showRowChange()
{
    for(int i = 0; i < tableModel->rowCount(); ++i)
    {
        ui->tableView_registerCourse->showRow(i);
    }
}



void Student_Interactive_Interface::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QObject::connect(timer,&QTimer::timeout,this,&Student_Interactive_Interface::overRegister);
    semesterChange(arg1);
    initAgain(arg1);
}

void Student_Interactive_Interface::on_action_i_m_t_kh_u_triggered()
{
    passWordChange->close();
    passWordChange->setNum(1);
    passWordChange->setUsername(getUsername());
    passWordChange->setPassWord(getPassword());
    passWordChange->show();
}

void Student_Interactive_Interface::displayMarkOfGraph(bool ch, int cl,QBarSet* bar)
{
    qDebug()<<"Index - > "<<cl;
    QFont fontt;
    fontt.setFamily("Courier");
    QPoint p = chartView->mapFromGlobal(QCursor::pos());
    QSqlQuery qury;
    qury.prepare("SELECT TENMONHOC,DIEMTONGKET,THANGDIEM4,DIEMCHU FROM MARK_OF_STUDENTS,SUBJECT_ WHERE MARK_OF_STUDENTS.MAMONHOC = SUBJECT_.MAMONHOC AND MSSV='"+getStuId()+"' AND SCHOOL_YEAR='"+ui->comboBox_yearSch->currentText()+"' ORDER BY TENMONHOC");
    qury.exec();
    qDebug()<<"Username : "<<getUsername();
    qDebug()<<"ID : "<<getStuId();
    while(qury.next()){
        if(bar->label() == qury.value(0).toString()){
            QToolTip::setFont(fontt);
            QToolTip::showText(chartView->mapToGlobal(QPoint(p)),tr("Môn học : "+qury.value(0).toString().toUtf8()+"\nThang điểm 10 : "+qury.value(1).toString().toUtf8()+"\nThang điểm 4 : "+qury.value(2).toString().toUtf8()+""));
            if(ch){bar->setBorderColor(QColor(255,170,0));}else{bar->setBorderColor(QColor(255,255,255));}
            break;
        }
    }
}

void Student_Interactive_Interface::initQMap()
{
    QSqlQuery qury;
    qDebug()<<"CUrrent date :"<<QDate::currentDate();
    qury.prepare("SELECT yearOfRes,timeOverRes FROM timeToRegis");
    qury.exec();
    while(qury.next()){
        qDebug()<<"DONEEEEEEEEEEEEEEEEEEEEEEE";
        mapp.insert(qury.value(0).toString(),qury.value(1).toString());
    }
    QMapIterator<QString,QString> ter(mapp);
    while(ter.hasNext()){
        qDebug()<<"NEXTTTTTTTTTTTTTTTTTTTTTTT";
        ter.next();
        qDebug()<<"Key - > "<<ter.key()<<" Value-> "<<ter.value();
    }
}


bool Student_Interactive_Interface::overRegister()
{
    QString date = ui->comboBox->currentText();
    QDate timeDate;
    QMapIterator<QString,QString> ter(mapp);
    while(ter.hasNext()){
        ter.next();
        if(ter.key() == date){

            timeDate = QDate::fromString(ter.value(),"yyyy-MM-dd");
            qDebug()<<"TIME DAYYYY ->"<<timeDate;

            if (timeDate <= QDate::currentDate() && date!=0){
                ui->pushButton_regis->setDisabled(true);
                ui->pushButton_cancelRegis->setDisabled(true);
                ui->tableView_registerCourse->setDisabled(true);
                ui->tableView_registeredCourse->setDisabled(true);
                ui->tableView_registerCourse->horizontalScrollBar()->hide();
                ui->tableView_registerCourse->verticalScrollBar()->hide();
                ui->tableView_registeredCourse->horizontalScrollBar()->hide();
                ui->tableView_registeredCourse->verticalScrollBar()->hide();
                messageBoxOneButton("Thông báo","Các lớp học phần này đã hết hạn đăng kí !","Đồng ý",350,100,80,30);
                QObject::disconnect(timer,&QTimer::timeout,this,&Student_Interactive_Interface::overRegister);
                return true;
            }
            ui->pushButton_regis->setDisabled(false);
            ui->pushButton_cancelRegis->setDisabled(false);
            ui->tableView_registerCourse->setDisabled(false);
            ui->tableView_registeredCourse->setDisabled(false);
            ui->tableView_registerCourse->horizontalScrollBar()->show();
            ui->tableView_registerCourse->verticalScrollBar()->show();
            ui->tableView_registeredCourse->verticalScrollBar()->show();
        }
    }
    QObject::disconnect(timer,&QTimer::timeout,this,&Student_Interactive_Interface::overRegister);
    return false;
}

void Student_Interactive_Interface::execQuery()
{
    QSqlQuery getStudentId;
    QString MSSV;
    getStudentId.prepare("SELECT MSSV FROM Student_DATABASE WHERE USERNAME = :USERNAME");
    getStudentId.bindValue(0,getUsername());
    getStudentId.exec();
    getStudentId.first();
    MSSV = getStudentId.value(0).toString();
    QString mark_y = NULL;
    QString subject_ = NULL;
    QString year_School = ui->comboBox_yearSch->currentText();
    subject_= ui->comboBox_subject->currentText();
    mark_y = year_School;
    QString qurr;
    qDebug()<<subject_<<" "<<mark_y;
    if (mark_y.isEmpty())
    {
        this->a = 1;
        qurr = QString("SELECT DISTINCT TENMONHOC,MARK_OF_STUDENTS.SCHOOL_YEAR,kiemTraLan1,kiemTraLan2,kiemTraLan3,DIEMTHUONGXUYEN,DIEMCUOIKI,DIEMTONGKET,THANGDIEM4,DIEMCHU,XEPLOAI FROM MARK_OF_STUDENTS,SUBJECT_ WHERE MSSV = '"+MSSV+"' AND MARK_OF_STUDENTS.MAMONHOC = SUBJECT_.MAMONHOC AND MARK_OF_STUDENTS.MAMONHOC = (SELECT SUBJECT_.MAMONHOC FROM SUBJECT_ WHERE SUBJECT_.TENMONHOC = N'"+subject_+"')");;
    }
    if (subject_.isEmpty())
    {
        this->a = -1;
        qurr = QString("SELECT DISTINCT TENMONHOC,MARK_OF_STUDENTS.SCHOOL_YEAR,kiemTraLan1,kiemTraLan2,kiemTraLan3,DIEMTHUONGXUYEN,DIEMCUOIKI,DIEMTONGKET,THANGDIEM4,DIEMCHU,XEPLOAI FROM MARK_OF_STUDENTS,SUBJECT_ WHERE MSSV = '"+MSSV+"' AND MARK_OF_STUDENTS.MAMONHOC = SUBJECT_.MAMONHOC AND MARK_OF_STUDENTS.SCHOOL_YEAR = '"+mark_y+"'");
    }
    if (mark_y.isEmpty() && subject_.isEmpty())
    {
        this->a = 1;
        qurr = QString("SELECT DISTINCT TENMONHOC,MARK_OF_STUDENTS.SCHOOL_YEAR,kiemTraLan1,kiemTraLan2,kiemTraLan3,DIEMTHUONGXUYEN,DIEMCUOIKI,DIEMTONGKET,THANGDIEM4,DIEMCHU,XEPLOAI FROM MARK_OF_STUDENTS,SUBJECT_ WHERE MSSV = '"+MSSV+"' AND MARK_OF_STUDENTS.MAMONHOC = SUBJECT_.MAMONHOC ORDER BY SCHOOL_YEAR ASC");
    }
    if (!mark_y.isEmpty() && !subject_.isEmpty())
    {
        this->a = -1;
        qurr = QString("SELECT DISTINCT TENMONHOC,MARK_OF_STUDENTS.SCHOOL_YEAR,kiemTraLan1,kiemTraLan2,kiemTraLan3,DIEMTHUONGXUYEN,DIEMCUOIKI,DIEMTONGKET,THANGDIEM4,DIEMCHU,XEPLOAI FROM MARK_OF_STUDENTS,SUBJECT_ WHERE MSSV = '"+MSSV+"' AND MARK_OF_STUDENTS.MAMONHOC = SUBJECT_.MAMONHOC AND MARK_OF_STUDENTS.SCHOOL_YEAR = '"+mark_y+"' AND MARK_OF_STUDENTS.MAMONHOC = (SELECT SUBJECT_.MAMONHOC FROM SUBJECT_ WHERE SUBJECT_.TENMONHOC = N'"+subject_+"')");//.arg(mark_y).arg(subject_);
    }
    qrymodall->setQuery(qurr);
    ui->tableView_mark->setModel(qrymodall);
    ui->tableView_mark->model()->setHeaderData(0,Qt::Horizontal,tr("Tên môn học"));
    ui->tableView_mark->model()->setHeaderData(1,Qt::Horizontal,tr("Năm học"));
    ui->tableView_mark->model()->setHeaderData(2,Qt::Horizontal,tr("Kiểm tra lần 1"));
    ui->tableView_mark->model()->setHeaderData(3,Qt::Horizontal,tr("Kiểm tra lần 2"));
    ui->tableView_mark->model()->setHeaderData(4,Qt::Horizontal,tr("Kiểm tra lần 3"));
    ui->tableView_mark->model()->setHeaderData(5,Qt::Horizontal,tr("Điểm thường xuyên"));
    ui->tableView_mark->model()->setHeaderData(6,Qt::Horizontal,tr("Điểm cuối kì"));
    ui->tableView_mark->model()->setHeaderData(7,Qt::Horizontal,tr("Điểm tổng kết"));
    ui->tableView_mark->model()->setHeaderData(8,Qt::Horizontal,tr("Thang điểm 4"));
    ui->tableView_mark->model()->setHeaderData(9,Qt::Horizontal,tr("Điểm chữ"));
    ui->tableView_mark->model()->setHeaderData(10,Qt::Horizontal,tr("Xếp loại"));
    ui->tableView_mark->setColumnWidth(0,250);
    ui->tableView_mark->setColumnWidth(1,120);
    ui->tableView_mark->setColumnWidth(2,150);
    ui->tableView_mark->setColumnWidth(3,150);
    ui->tableView_mark->setColumnWidth(4,150);
    ui->tableView_mark->setColumnWidth(5,150);
    ui->tableView_mark->setColumnWidth(6,120);
    ui->tableView_mark->setColumnWidth(7,120);
    ui->tableView_mark->setColumnWidth(8,120);
    ui->tableView_mark->setColumnWidth(9,120);
    ui->tableView_mark->setColumnWidth(10,120);
    if (this->a == 1){qDebug()<<"1";ui->tableView_mark->showColumn(1);}
    if(this->a == -1){qDebug()<<"-1";ui->tableView_mark->hideColumn(1);}
}

void Student_Interactive_Interface::closeEvent(QCloseEvent *event)
{
    if(checkClose){
        bool check = messageBoxTwoButton("Thông báo !","Bạn có chắc muốn thoát chương trình !","Đồng ý","Hủy",250,150,80,28);
        if(check){event->accept();qApp->exit();}else{event->ignore();}
    }
}

void Student_Interactive_Interface::initGraph()
{
    QString yee = ui->comboBox_yearSch->currentText();
    series = new QBarSeries();
    if(yee.isEmpty()){
        yee = ui->comboBox_yearSch->itemText(1);
    }
    if(vecBar.size() != 0){vecBar.clear();}
    if(vecBar.size() == 0){
        QSqlQuery qury;int z=0;
        qury.prepare("SELECT TENMONHOC,DIEMTONGKET,THANGDIEM4,DIEMCHU FROM MARK_OF_STUDENTS,SUBJECT_ WHERE MARK_OF_STUDENTS.MAMONHOC = SUBJECT_.MAMONHOC AND MSSV='"+getStuId()+"' AND SCHOOL_YEAR='"+yee+"' ORDER BY TENMONHOC");
        qury.exec();

        while(qury.next()){
            set2 = new QBarSet(qury.value(0).toString());
            qDebug()<<"Label -> "<<set2->label();
            vecBar.push_back(set2);
            ++z;
        }

        if(z>=7){series->setBarWidth(1);}else{if(z==1){series->setBarWidth(0.25);}series->setBarWidth(0.5);}

        qury.exec();
        int i = 0;
        while(qury.next()){
            *vecBar[i]<<qury.value(1).toInt();
            i++;
        }

        for(unsigned long long i = 0;i<vecBar.size();++i){
            vecBar[i]->setColor(vecColor[i]);
            series->append(vecBar[i]);
        }

        QObject::connect(series,&QAbstractBarSeries::hovered,this,&Student_Interactive_Interface::displayMarkOfGraph);
        chart = new QChart();
        QFont fontt;fontt.setFamily("Courier");fontt.setBold(true);fontt.setPixelSize(12);
        chart->addSeries(series);
        chart->setTitle("BIỂU ĐỒ ĐIỂM NĂM HỌC "+yee+"");
        chart->setTitleFont(fontt);
        chart->setTitleBrush(QBrush(QColor(0,0,0)));
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QLinearGradient plotAreaGradient;
        plotAreaGradient.setStart(QPointF(0, 1));
        plotAreaGradient.setFinalStop(QPointF(1, 0));
        plotAreaGradient.setColorAt(0.0,QColor(255,170,0));
        plotAreaGradient.setColorAt(1.0,QColor(255,170,0));
        plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        chart->setPlotAreaBackgroundBrush(plotAreaGradient);
        chart->setPlotAreaBackgroundVisible(true);


        QStringList categories;
        categories <<"";
        axisX = new QBarCategoryAxis();
        axisX->append(categories);

        axisY = new QValueAxis();
        axisY->setRange(0,10);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);


        chart->legend()->hide();
        chart->legend()->setAlignment(Qt::AlignBottom);

        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        chartView->setFixedSize(340,350);
        chartView->setParent(ui->horizontalFrame);
        //QObject::connect(series,&QAbstractBarSeries::hovered,this,&Student_Interactive_Interface::displayMarkOfGraph);
        chartView->show();
    }
}

void Student_Interactive_Interface::setStuId(QString a){this->myStuId = a;}

void Student_Interactive_Interface::printSchedule()
{
    QXlsx::Document xlxs;
    Format format,formatCellHeader,formatCell;

    format.setFontBold(true);format.setFontColor(QColor(0,0,0));
    format.setFontName("Calibri");format.setHorizontalAlignment(Format::AlignHCenter);format.setFontSize(18);
    format.setPatternBackgroundColor(QColor(255,170,0));format.setVerticalAlignment(Format::AlignVCenter);

    formatCellHeader.setFontName("Calibri");formatCellHeader.setPatternBackgroundColor(QColor(255,170,0));formatCellHeader.setFontBold(false);
    formatCellHeader.setFontSize(13);formatCellHeader.setBorderStyle(Format::BorderThin);formatCellHeader.setBorderColor(QColor(0,0,0));
    formatCellHeader.setHorizontalAlignment(Format::AlignHCenter);formatCellHeader.setVerticalAlignment(Format::AlignVCenter);

    formatCell.setFontName("Calibri");formatCell.setPatternBackgroundColor(QColor(196,215,155));
    formatCell.setFontSize(16);formatCell.setBorderStyle(Format::BorderThin);formatCell.setBorderStyle(Format::BorderThin);
    formatCell.setHorizontalAlignment(Format::AlignHCenter);formatCell.setVerticalAlignment(Format::AlignVCenter);

    xlxs.mergeCells(CellRange("E3:G3"));
    if(this->index == 1){xlxs.write("E3","THỜI KHÓA BIỂU",format);}
    if(this->index == 2){xlxs.write("E3","LỊCH THI",format);}
    xlxs.setRowHeight(3,30);
    xlxs.setRowHeight(6,20);
    formatCell.setPatternBackgroundColor(QColor(182, 182, 136));
    xlxs.mergeCells(CellRange("B9:B12"),formatCell);xlxs.mergeCells(CellRange("B14:B17"),formatCell);xlxs.mergeCells(CellRange("B19:B22"),formatCell);
    xlxs.write("B9","Sáng",formatCell);
    xlxs.write("B14","Trưa",formatCell);
    xlxs.write("B19","Tối",formatCell);
    formatCell.setBorderStyle(Format::BorderNone);
    xlxs.mergeCells("B4:C4",formatCell);
    xlxs.write("B4"," Môn học : S, Phòng học : P ",formatCell);

    xlxs.setColumnWidth(2,20);
    xlxs.setColumnWidth(3,20);
    xlxs.setColumnWidth(4,20);
    xlxs.setColumnWidth(5,20);
    xlxs.setColumnWidth(6,20);
    xlxs.setColumnWidth(7,20);
    xlxs.setColumnWidth(8,20);
    xlxs.setColumnWidth(9,20);

    xlxs.write("C6","Thứ 2",formatCellHeader);
    xlxs.write("D6","Thứ 3",formatCellHeader);
    xlxs.write("E6","Thứ 4",formatCellHeader);
    xlxs.write("F6","Thứ 5",formatCellHeader);
    xlxs.write("G6","Thứ 6",formatCellHeader);
    xlxs.write("H6","Thứ 7",formatCellHeader);
    xlxs.write("I6","Chủ nhật",formatCellHeader);
    formatCellHeader.setPatternBackgroundColor(QColor(243,247,249));formatCellHeader.setVerticalAlignment(Format::AlignVCenter);
    xlxs.mergeCells(CellRange("B6:B7"),formatCellHeader);
    xlxs.write("B6","Ca học",formatCellHeader);
    xlxs.write("C7",ui->label_day2->text(),formatCellHeader);
    xlxs.write("D7",ui->label_day3->text(),formatCellHeader);
    xlxs.write("E7",ui->label_day4->text(),formatCellHeader);
    xlxs.write("F7",ui->label_day5->text(),formatCellHeader);
    xlxs.write("G7",ui->label_day6->text(),formatCellHeader);
    xlxs.write("H7",ui->label_day7->text(),formatCellHeader);
    xlxs.write("I7",ui->label_day8->text(),formatCellHeader);

    formatCell.setFontSize(13);formatCell.setPatternBackgroundColor(QColor(255, 170, 0));formatCell.setBorderStyle(Format::BorderThin);formatCell.setTextWrap(true);
    if(!ui->label_m2->text().isEmpty()){xlxs.mergeCells(CellRange("C9:C12"),formatCell);xlxs.write("C9",ui->label_m2->text(),formatCell);}else{xlxs.mergeCells(CellRange("C9:C12"));}
    if(!ui->label_m3->text().isEmpty()){xlxs.mergeCells(CellRange("D9:D12"),formatCell);xlxs.write("D9",ui->label_m3->text(),formatCell);}else{xlxs.mergeCells(CellRange("D9:D12"));}
    if(!ui->label_m4->text().isEmpty()){xlxs.mergeCells(CellRange("E9:E12"),formatCell);xlxs.write("E9",ui->label_m4->text(),formatCell);}else{xlxs.mergeCells(CellRange("E9:E12"));}
    if(!ui->label_m5->text().isEmpty()){xlxs.mergeCells(CellRange("F9:F12"),formatCell);xlxs.write("F9",ui->label_m5->text(),formatCell);}else{xlxs.mergeCells(CellRange("F9:F12"));}
    if(!ui->label_m6->text().isEmpty()){xlxs.mergeCells(CellRange("G9:G12"),formatCell);xlxs.write("G9",ui->label_m6->text(),formatCell);}else{xlxs.mergeCells(CellRange("G9:G12"));}
    if(!ui->label_m7->text().isEmpty()){xlxs.mergeCells(CellRange("H9:H12"),formatCell);xlxs.write("H9",ui->label_m7->text(),formatCell);}else{xlxs.mergeCells(CellRange("H9:H12"));}
    if(!ui->label_m8->text().isEmpty()){xlxs.mergeCells(CellRange("I9:I12"),formatCell);xlxs.write("I9",ui->label_m8->text(),formatCell);}else{xlxs.mergeCells(CellRange("I9:I12"));}
    if(!ui->label_n2->text().isEmpty()){xlxs.mergeCells(CellRange("C14:C17"),formatCell);xlxs.write("C14",ui->label_n2->text(),formatCell);}else{xlxs.mergeCells(CellRange("C14:C17"));}
    if(!ui->label_n3->text().isEmpty()){xlxs.mergeCells(CellRange("D14:D17"),formatCell);xlxs.write("D14",ui->label_n3->text(),formatCell);}else{xlxs.mergeCells(CellRange("D14:D17"));}
    if(!ui->label_n4->text().isEmpty()){xlxs.mergeCells(CellRange("E14:E17"),formatCell);xlxs.write("E14",ui->label_n4->text(),formatCell);}else{xlxs.mergeCells(CellRange("E14:E17"));}
    if(!ui->label_n5->text().isEmpty()){xlxs.mergeCells(CellRange("F14:F17"),formatCell);xlxs.write("F14",ui->label_n5->text(),formatCell);}else{xlxs.mergeCells(CellRange("F14:F17"));}
    if(!ui->label_n6->text().isEmpty()){xlxs.mergeCells(CellRange("G14:G17"),formatCell);xlxs.write("G14",ui->label_n6->text(),formatCell);}else{xlxs.mergeCells(CellRange("G14:G17"));}
    if(!ui->label_n7->text().isEmpty()){xlxs.mergeCells(CellRange("H14:H17"),formatCell);xlxs.write("H14",ui->label_n7->text(),formatCell);}else{xlxs.mergeCells(CellRange("H14:H17"));}
    if(!ui->label_n8->text().isEmpty()){xlxs.mergeCells(CellRange("I14:I17"),formatCell);xlxs.write("I14",ui->label_n8->text(),formatCell);}else{xlxs.mergeCells(CellRange("I14:I17"));}
    if(!ui->label_ni2->text().isEmpty()){xlxs.mergeCells(CellRange("C19:C22"),formatCell);xlxs.write("C19",ui->label_ni2->text(),formatCell);}else{xlxs.mergeCells(CellRange("C19:C22"));}
    if(!ui->label_ni3->text().isEmpty()){xlxs.mergeCells(CellRange("D19:D22"),formatCell);xlxs.write("D19",ui->label_ni3->text(),formatCell);}else{xlxs.mergeCells(CellRange("D19:D22"));}
    if(!ui->label_ni4->text().isEmpty()){xlxs.mergeCells(CellRange("E19:E22"),formatCell);xlxs.write("E19",ui->label_ni4->text(),formatCell);}else{xlxs.mergeCells(CellRange("E19:E22"));}
    if(!ui->label_ni5->text().isEmpty()){xlxs.mergeCells(CellRange("F19:F22"),formatCell);xlxs.write("F19",ui->label_ni5->text(),formatCell);}else{xlxs.mergeCells(CellRange("F19:F22"));}
    if(!ui->label_ni6->text().isEmpty()){xlxs.mergeCells(CellRange("G19:G22"),formatCell);xlxs.write("G19",ui->label_ni6->text(),formatCell);}else{xlxs.mergeCells(CellRange("G19:G22"));}
    if(!ui->label_ni7->text().isEmpty()){xlxs.mergeCells(CellRange("H19:H22"),formatCell);xlxs.write("H19",ui->label_ni7->text(),formatCell);}else{xlxs.mergeCells(CellRange("H19:H22"));}
    if(!ui->label_ni8->text().isEmpty()){xlxs.mergeCells(CellRange("I19:I22"),formatCell);xlxs.write("I19",ui->label_ni8->text(),formatCell);}else{xlxs.mergeCells(CellRange("I19:I22"));}


    QString setFill = QString("Excel (*.xlsx)");
    QString fileNamePath = QFileDialog::getSaveFileName(this,"Lưu file","",setFill);
    bool saveCheck = xlxs.saveAs(fileNamePath);QString dd,nn;
    if(this->index == 1){dd="Xuất thời khóa biểu thành công !";nn="Xuất thời khóa biểu thất bại !";}
    if(this->index == 2){dd="Xuất lịch thi thành công !";nn="Xuất lịch thi thất bại !";}
    if(saveCheck){messageBoxOneButton("Thông báo !",dd,"Đồng ý",250,150,80,28);}
    else{
        messageBoxOneButton("Thông báo !",nn,"Đồng ý",250,150,80,28);}
}

void Student_Interactive_Interface::printMark()
{
    int x,y,q=5;x = ui->tableView_mark->model()->rowCount();y = ui->tableView_mark->model()->columnCount();
    QXlsx::Document xlxs;QString aa,aaa;
    Format format,formatCellHeader,formatCell;
    format.setFontBold(true);format.setFontColor(QColor(0,0,0));
    format.setFontName("Calibri");format.setHorizontalAlignment(Format::AlignHCenter);format.setFontSize(18);
    format.setPatternBackgroundColor(QColor(255,170,0));format.setVerticalAlignment(Format::AlignVCenter);

    formatCellHeader.setFontName("Calibri");formatCellHeader.setPatternBackgroundColor(QColor(255,170,0));formatCellHeader.setFontBold(true);
    formatCellHeader.setFontSize(13);formatCellHeader.setBorderStyle(Format::BorderThin);formatCellHeader.setBorderColor(QColor(0,0,0));
    formatCellHeader.setHorizontalAlignment(Format::AlignHCenter);formatCellHeader.setVerticalAlignment(Format::AlignVCenter);

    formatCell.setFontName("Calibri");formatCell.setPatternBackgroundColor(QColor(196,215,155));formatCell.setFontBold(false);
    formatCell.setFontSize(12);formatCell.setBorderStyle(Format::BorderThin);formatCell.setBorderColor(QColor(0,0,0));
    formatCell.setHorizontalAlignment(Format::AlignHCenter);formatCell.setVerticalAlignment(Format::AlignVCenter);

    xlxs.mergeCells(CellRange("E3:G3"));
    xlxs.write("E3","DANH SÁCH ĐIỂM",format);
    xlxs.setRowHeight(3,30);
    xlxs.setRowHeight(6,20);

    xlxs.setColumnWidth(1,25);
    xlxs.setColumnWidth(2,20);
    xlxs.setColumnWidth(3,15);
    xlxs.setColumnWidth(4,20);
    xlxs.setColumnWidth(5,20);
    xlxs.setColumnWidth(6,23);
    xlxs.setColumnWidth(7,20);
    xlxs.setColumnWidth(8,28);
    xlxs.setColumnWidth(9,20);
    xlxs.setColumnWidth(10,20);
    xlxs.setColumnWidth(11,15);

    xlxs.write("A6","Tên môn học",formatCellHeader);
    xlxs.write("B6","Năm học",formatCellHeader);
    xlxs.write("C6","Kiểm tra lần 1",formatCellHeader);
    xlxs.write("D6","Kiểm tra lần 2",formatCellHeader);
    xlxs.write("E6","Kiểm tra lần 3",formatCellHeader);
    xlxs.write("F6","Điểm thường xuyên",formatCellHeader);
    xlxs.write("G6","Điểm cuối kì",formatCellHeader);
    xlxs.write("H6","Điểm tổng kết",formatCellHeader);
    xlxs.write("I6","Thang điểm 4",formatCellHeader);
    xlxs.write("J6","Điểm chữ",formatCellHeader);
    xlxs.write("K6","Xếp loại",formatCellHeader);

    qDebug()<<"Combox Text  >"<<comboxYear->currentText();
    for (int i = 0 ;i < x ; ++i){
        aa = ui->tableView_mark->model()->index(i,1,QModelIndex()).data().toString();
        qDebug()<<"Year->"<<aa;
        if(aa == comboxYear->currentText()){
            ++q;
            for(int j = 0;j < y; ++j){
                aaa = ui->tableView_mark->model()->index(i,j,QModelIndex()).data().toString();
                xlxs.write(q+1,j+1,aaa,formatCell);
                qDebug()<<aaa<<" "<<i<<" "<<j<<" "<<getStuId();
            }
        }
    }

    QString setFill = QString("Excel (*.xlsx)");
    QString fileNamePath = QFileDialog::getSaveFileName(this,"Lưu file","",setFill);
    bool saveCheck = xlxs.saveAs(fileNamePath);
    if(saveCheck){widget->close();messageBoxOneButton("Thông báo !","Xuất file điểm thành công !","Đồng ý",250,150,80,28);}
    else{widget->close();messageBoxOneButton("Thông báo !","Xuất file điểm thất bại, thử lại sau !","Đồng ý",250,150,80,28);}
}

void Student_Interactive_Interface::createWidgetForOptionPrints()
{
    widget->setWindowTitle("Lựa chọn đi nào !");
    widget->setWindowIcon(QIcon(":/icon-file/more_options_standard.png"));
    QSqlQuery qry,query;
    qDebug()<<"MSSV - >>"<<getStuId();
    qry.prepare("SELECT DISTINCT SCHOOL_YEAR FROM MARK_OF_STUDENTS WHERE MSSV='"+getStuId()+"' ORDER BY SCHOOL_YEAR DESC");
    qry.exec();
    comboxYear->clear();
    while(qry.next()){
        comboxYear->addItem(qry.value(0).toString());
    }
    labelYear->setFixedSize(70,28);
    labelYear->setStyleSheet("font : 8pt Courier");comboxYear->setFixedSize(80,28);conti->setFixedSize(90,30);conti->setFocusPolicy(Qt::NoFocus);
    conti->setFont(QFont("Courier"));
    conti->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);height : 25px;width : 100px;border-radius : 8px;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:pressed{background-color: rgb(255, 170, 0);border:1px groove rgb(68, 68, 68);outline : 0px;}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0px;}");
    comboxYear->setStyleSheet("QComboBox{subcontrol-origin: padding;subcontrol-position: top right;selection-background-color :rgb(255, 170, 0);background-color:white;selection-color: black;color: black;border-style: solid;border: 2px solid rgb(88, 88, 88);border-radius: 6;padding: 1px 0px 1px 20px;}"
    "QComboBox:hover{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:focus{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:on{border-radius : 5px;padding-top: 0px;padding-left: 0px;selection-background-color: rgb(255, 170, 0);color: black;selection-color :black;border : 1px solid rgb(84, 84, 84);}"
    "QComboBox:!on{color: black;}"
    "QComboBox QAbstractItemView{border: 2px solid darkgray;background-color:white;selection-color: black;selection-background-color: rgb(255, 170, 0);outline : 0;}"
    "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;width: 15px;color: white;border-left-width: 0px;border-left-color: darkgray;border-left-style: solid; border-top-right-radius: 6px;border-bottom-right-radius: 6px;padding-left: 10px;}"
    "QComboBox::down-arrow{width: 7px;height: 5px;}"
    "QComboBox QAbstractItemView::item:selected{border : 1px groove rgb(108, 108, 108);background-color: rgb(170, 170, 127);color : black;selection-color:black;}"
    "QComboBox QAbstractItemView::item{border : 1px groove rgb(108, 108, 108);color : black;selection-color:black;}");
    hLayout->addWidget(labelYear);hLayout->addWidget(comboxYear);
    hLayout->addWidget(conti);
    hLayout->setGeometry(QRect(10,10,250,100));
    widget->setWindowModality(Qt::WindowModal);
    widget->setFixedSize(350,110);
    widget->setLayout(hLayout);
    widget->show();
}

void Student_Interactive_Interface::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F5){
        qDebug()<<"IT DIDDD";
        initComboBox();
        semesterChange(ui->comboBox->currentText());
        tableModelDebt->select();
    }
}


QString Student_Interactive_Interface::getStuId(){return this->myStuId;}

void Student_Interactive_Interface::on_comboBox_subject_currentIndexChanged(int index){execQuery();}
void Student_Interactive_Interface::on_comboBox_yearSch_currentIndexChanged(int index){initGraph();execQuery();}


void Student_Interactive_Interface::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
    {
        ui->label_namhoc->hide();
        ui->label_monhoc->hide();
        ui->comboBox_subject->hide();
        ui->comboBox_yearSch->hide();
        ui->comboBox_options->hide();
        ui->pushButton_xem->hide();
        ui->label_9->hide();
        ui->label_10->hide();
        ui->label_11->hide();
        ui->spacerI->changeSize(20,80);
        ui->horizontalFrame->hide();
    }
    if(index == 1)
    {
        ui->label_namhoc->show();
        ui->label_monhoc->show();
        ui->comboBox_subject->show();
        ui->comboBox_yearSch->show();
        ui->comboBox_options->show();
        ui->pushButton_xem->show();
        ui->label_9->show();
        ui->label_10->show();
        ui->label_11->show();
        ui->spacerI->changeSize(20,30);
        ui->verticalSpacer_11->changeSize(20,50);
        ui->horizontalFrame->show();
    }
    else if (index == 2)
    {
        ui->label_namhoc->show();
        ui->label_monhoc->show();
        ui->comboBox_subject->show();
        ui->comboBox_yearSch->show();
        ui->comboBox_options->show();
        ui->pushButton_xem->show();
        ui->label_9->show();
        ui->label_10->show();
        ui->label_11->show();
        ui->spacerI->changeSize(20,30);
        ui->verticalSpacer_11->changeSize(20,50);
        ui->horizontalFrame->hide();
        //this->index = 1;
    }
    else if (index == 3)
    {
        ui->label_namhoc->show();
        ui->label_monhoc->show();
        ui->comboBox_subject->show();
        ui->comboBox_yearSch->show();
        ui->comboBox_options->show();
        ui->pushButton_xem->show();
        ui->label_9->show();
        ui->label_10->show();
        ui->label_11->show();
        ui->spacerI->changeSize(20,30);
        ui->verticalSpacer_11->changeSize(20,50);
        ui->horizontalFrame->hide();
    }
    else{
        ui->label_namhoc->hide();
        ui->label_monhoc->hide();
        ui->comboBox_subject->hide();
        ui->comboBox_yearSch->hide();
        ui->comboBox_options->hide();
        ui->pushButton_xem->hide();
        ui->label_9->hide();
        ui->label_10->hide();
        ui->label_11->hide();
        ui->spacerI->changeSize(20,80);
        ui->horizontalFrame->hide();
    }
}


void Student_Interactive_Interface::on_actionIn_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_study);
    createWidgetForOptionPrints();
}


void Student_Interactive_Interface::on_pushButton_5_clicked()
{
            if(this->index == 1){
                getIndex(1);
                printSchedule();
            }

            if(this->index == 2){
                getIndex(2);
                printSchedule();
            }
}



void Student_Interactive_Interface::on_actionKh_i_ng_l_i_triggered()
{
    bool check = messageBoxTwoButton("Thông báo !","Bạn có chắc khởi động lại chương trình !","Đồng ý","Hủy",250,150,80,28);
    if(check){
        checkClose = false;
        this->close();
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

