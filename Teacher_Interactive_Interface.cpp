#include "Teacher_Interactive_Interface.h"
#include "ui_Teacher_Interactive_Interface.h"
#include<QDate>
#include<QEvent>

Teacher_Interactive_Interface::Teacher_Interactive_Interface(QWidget *parent,bool pass,QString username,QString password) :
    QMainWindow(parent),
    ui(new Ui::Teacher_Interactive_Interface)
{
    ui->setupUi(this);
    setUsername(username);
    setPassWord(password);
    timer = new QTimer();
    widget = new QWidget();
    calendar = new QCalendarWidget();
    tableModel = new QSqlTableModel(this);
    tableListOffer = new QSqlTableModel();
    tableScheduleOffer = new QSqlTableModel();
    itemModel = new QStandardItemModel(this);
    changeMark = new ChangeMarkOfStudent();
    changeInfo = new ChangeInformationOfStudent();
    changePW = new ChangePassword(nullptr,getUsername(),getPassword());
    quryModel = new QSqlQueryModel();

    hLayout = new QHBoxLayout();labelYear = new QLabel("Năm học :");labelClass = new QLabel("    Lớp :");itemSpacer = new QSpacerItem(20,20,QSizePolicy::Fixed,QSizePolicy::Fixed);
    comboxYear = new QComboBox();comboxClass = new QComboBox();vLayout = new QVBoxLayout();conti = new QPushButton("Bắt đầu in");


    QObject::connect(calendar,&QCalendarWidget::selectionChanged,this,&Teacher_Interactive_Interface::onSelectionChanged);
    QObject::connect(changeMark,&ChangeMarkOfStudent::destroyed,this,&Teacher_Interactive_Interface::showText);
    QObject::connect(changeInfo,&ChangeInformationOfStudent::destroyed,this,&Teacher_Interactive_Interface::updateInfo);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(setTime()));
    QObject::connect(conti,&QPushButton::clicked,this,&Teacher_Interactive_Interface::doPrint);
    QObject::connect(comboxClass,&QComboBox::currentTextChanged,this,&Teacher_Interactive_Interface::doWhenComboBoxChanged);
    QObject::connect(ui->comboBox_year,&QComboBox::currentTextChanged,this,&Teacher_Interactive_Interface::doWhenUiComboBoxYearChange);
    QObject::connect(ui->comboBox_ye,&QComboBox::currentTextChanged,this,&Teacher_Interactive_Interface::whenYearOfCourseChange);

    ui->lineEdit_dateEnd->setFocus();
    ui->lineEdit_dateTest->setFocus();

    timer->start(1000);
    setIndex(1);
    Init();

    initTableRes();
    indexx = 1;
    this->index = 1;
    setPictureForUser();
    readFile();
    setSaveOrNot(pass);
    checkClose = true;

    ui->lineEdit_courseId->setMaxLength(10);
    ui->lineEdit_dateStart->setMaxLength(10);
    ui->lineEdit_dateEnd->setMaxLength(10);
    ui->lineEdit_dateTest->setMaxLength(10);
    ui->lineEdit_semester->setMaxLength(4);
    ui->lineEdit_quantity->setMaxLength(2);
    ui->lineEdit_numRes->setMaxLength(1);

    initTableScheduleOffer();
    initTableListCourseOffer();

    ui->lineEdit_dateStu->setAlignment(Qt::AlignHCenter);
    ui->label_18->hide();
    ui->tableView_ScheduleCourse->hide();
}

Teacher_Interactive_Interface::~Teacher_Interactive_Interface()
{
    delete ui;
    delete calendar;
    delete tableModel;delete tableListOffer;delete tableScheduleOffer;
    delete itemModel;
    delete changeMark;
    delete changeInfo;
    delete changePW;
    delete quryModel;
    delete timer;
    delete widget;delete conti;delete itemSpacer;
    delete hLayout;delete labelYear;delete labelClass;delete comboxYear;delete comboxClass;delete vLayout;
}

void Teacher_Interactive_Interface::Init()
{
    ui->tabWidget->setCurrentWidget(ui->tab_information);
    ui->tableView->verticalHeader()->hide();xRe = 3;
    ui->tableView->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_ResStu->horizontalHeader()->setSectionsClickable(false);
    ui->comboBox_op->hide();
    ui->radioButton_CT->hide();
    ui->radioButton_TC->hide();
    ui->pushButton_9->hide();ui->pushButton_10->hide();ui->pushButton_11->hide();ui->pushButton_12->hide();ui->pushButton_13->hide();
    ui->label_8->hide();ui->label_9->hide();ui->comboBox_class->hide();ui->comboBox_year->hide();
    qDebug()<<this->getUsername()<<":"<<this->getPassword();
    QSqlQuery query,qry;
    query.prepare("SELECT * FROM Lecturer_DATABASE WHERE USERNAME = :USERNAME AND PASSWORD = :PASSWORD");
    query.bindValue(0,getUsername());
    query.bindValue(1,getPassword());
    query.exec();
    query.first();
    qDebug()<<query.value(2).toString().toStdString().c_str();
    qry.prepare("SELECT _thuocKhoa.tenKhoa FROM _thuocKhoa WHERE _thuocKhoa.tenLop = :TENLOP ");
    qry.bindValue(0,query.value(2).toString());
    qry.exec();
    qry.first();
    if (getUsername() == query.value(9).toString() && getPassword() == query.value(10).toString())
    {
        QDate date_ = QDate::fromString(query.value(8).toString(),"yyyy-MM-dd");
        ui->label_name->setText("Họ tên : "+query.value(1).toString());
        ui->label_sex->setText("Giới tính : "+ query.value(7).toString());
        ui->label_phonenumber->setText("Số điện thoại : "+query.value(4).toString());
        ui->label_email->setText("Email : "+query.value(5).toString());
        ui->label_datebirth->setText("Ngày sinh : "+date_.toString("dd-MM-yyyy"));
        ui->label_identitycard->setText("Căn cước công dân : "+query.value(3).toString());
        ui->label_id->setText("Mã giáo viên : "+query.value(0).toString());
        ui->label_profess->setText("Môn dạy : "+query.value(2).toString());
        ui->label_address->setText("Địa chỉ : "+query.value(6).toString());
        if(!query.value(11).toString().isEmpty()){ui->label_ethnic->setText("Dân tộc : "+query.value(11).toString());}
        if(!query.value(12).toString().isEmpty()){ui->label_placeofbirth->setText("Nơi sinh : "+query.value(12).toString());}
        ui->label_khoa->setText("Khoa : "+qry.value(0).toString());
    }
    msgv=query.value(0).toString();
    changeMark->setSubId(query.value(2).toString());
    QSqlQuery de;
    de.prepare("SELECT SUBJECT_.MAMONHOC FROM SUBJECT_ WHERE SUBJECT_.TENMONHOC =N'"+query.value(2).toString()+"'");
    de.exec();
    de.first();
    this->professional = de.value(0).toString();
    qDebug()<<"First Major :"<<de.value(0).toString();
    changeMark->setPro(this->professional);
    changeInfo->setIdd(query.value(0).toString());
    initTableMark();
    initComboBox();
    inittabletime(QDate::currentDate());
    QLabel* label = new QLabel("");
    label->setFixedWidth(33);
    label->setFixedHeight(130);
    ui->toolBar->insertWidget(ui->actionTh_ng_tin_c_c_nh_n,label);
    this->setWindowFlags(Qt::Window);
    ui->verticalSpacer_5->changeSize(20,400);

    QSqlQuery qury;
    qury.prepare("SELECT DISTINCT semester FROM listCourseOffer WHERE subjectID='"+this->professional+"' ORDER BY semester DESC");
    qury.exec();
    while(qury.next()){
        ui->comboBox_ye->addItem(qury.value(0).toString());
    }
}

void Teacher_Interactive_Interface::InitResStu()
{
    qDebug()<<"Major : "<<this->professional;
    if(!ui->comboBox_year->currentText().isEmpty()){ui->tableView_ResStu->hideColumn(0);}
    if(!ui->comboBox_class->currentText().isEmpty()){ui->tableView_ResStu->hideColumn(1);}
    if(ui->comboBox_year->currentText().isEmpty()){ui->tableView_ResStu->showColumn(0);}
    if(ui->comboBox_class->currentText().isEmpty()){ui->tableView_ResStu->showColumn(1);}
    if(!ui->comboBox_year->currentText().isEmpty() && ui->comboBox_class->currentText().isEmpty())
    {
        quryModel->setQuery("SELECT semester,courseCode,MSSV,NAME,CLASSNAME,MAJOR,GMAIL,SEX,PHONENUMBER,CMND FROM Student_DATABASE,registered_course WHERE Student_DATABASE.MSSV = registered_course.studentID AND subjectID='"+this->professional+"' AND registered_course.semester='"+ui->comboBox_year->currentText()+"' ORDER BY registered_course.courseCode");
        ui->tableView_ResStu->hideColumn(0);
        ui->tableView_ResStu->showColumn(1);
    }
    if(ui->comboBox_year->currentText().isEmpty() && !ui->comboBox_class->currentText().isEmpty())
    {
        quryModel->setQuery("SELECT semester,courseCode,MSSV,NAME,CLASSNAME,MAJOR,GMAIL,SEX,PHONENUMBER,CMND FROM Student_DATABASE,registered_course WHERE Student_DATABASE.MSSV = registered_course.studentID AND subjectID='"+this->professional+"' AND registered_course.courseCode='"+ui->comboBox_class->currentText()+"' ORDER BY semester");
        ui->tableView_ResStu->hideColumn(1);
        ui->tableView_ResStu->showColumn(0);
    }
    if(ui->comboBox_year->currentText().isEmpty() && ui->comboBox_class->currentText().isEmpty())
    {
        quryModel->setQuery("SELECT semester,courseCode,MSSV,NAME,CLASSNAME,MAJOR,GMAIL,SEX,PHONENUMBER,CMND FROM Student_DATABASE,registered_course WHERE Student_DATABASE.MSSV = registered_course.studentID AND subjectID='"+this->professional+"' ORDER BY semester");
        ui->tableView_ResStu->showColumn(0);ui->tableView_ResStu->showColumn(1);
    }
    if(!ui->comboBox_year->currentText().isEmpty() && !ui->comboBox_class->currentText().isEmpty())
    {
        quryModel->setQuery("SELECT semester,courseCode,MSSV,NAME,CLASSNAME,MAJOR,GMAIL,SEX,PHONENUMBER,CMND FROM Student_DATABASE,registered_course WHERE Student_DATABASE.MSSV = registered_course.studentID AND subjectID='"+this->professional+"' AND registered_course.semester='"+ui->comboBox_year->currentText()+"' AND registered_course.courseCode='"+ui->comboBox_class->currentText()+"' ORDER BY registered_course.courseCode");
        ui->tableView_ResStu->hideColumn(0);ui->tableView_ResStu->hideColumn(0);
    }
    ui->tableView_ResStu->setModel(quryModel);
    ui->tableView_ResStu->verticalHeader()->hide();
    ui->tableView_ResStu->model()->setHeaderData(0,Qt::Horizontal,tr("Năm học"));
    ui->tableView_ResStu->model()->setHeaderData(1,Qt::Horizontal,tr("Mã môn học"));
    ui->tableView_ResStu->model()->setHeaderData(2,Qt::Horizontal,tr("Mã sinh viên"));
    ui->tableView_ResStu->model()->setHeaderData(3,Qt::Horizontal,tr("Họ tên"));
    ui->tableView_ResStu->model()->setHeaderData(4,Qt::Horizontal,tr("Lớp"));
    ui->tableView_ResStu->model()->setHeaderData(5,Qt::Horizontal,tr("Ngành học"));
    ui->tableView_ResStu->model()->setHeaderData(6,Qt::Horizontal,tr("Email"));
    ui->tableView_ResStu->model()->setHeaderData(7,Qt::Horizontal,tr("Giới tính"));
    ui->tableView_ResStu->model()->setHeaderData(8,Qt::Horizontal,tr("Số điện thoại"));
    ui->tableView_ResStu->model()->setHeaderData(9,Qt::Horizontal,tr("Căn cước công dân"));
    ui->tableView_ResStu->setColumnWidth(2,130);
    ui->tableView_ResStu->setColumnWidth(3,180);
    ui->tableView_ResStu->setColumnWidth(5,310);
    ui->tableView_ResStu->setColumnWidth(6,210);
    ui->tableView_ResStu->setColumnWidth(8,150);
    ui->tableView_ResStu->setColumnWidth(9,150);
}

void Teacher_Interactive_Interface::initTableRes()
{
    quryModel->setQuery("SELECT semester,courseCode,MSSV,NAME,CLASSNAME,MAJOR,GMAIL,SEX,PHONENUMBER,CMND FROM Student_DATABASE,registered_course WHERE Student_DATABASE.MSSV = registered_course.studentID AND subjectID='"+this->professional+"' ORDER BY registered_course.courseCode");
    ui->tableView_ResStu->setModel(quryModel);
    ui->tableView_ResStu->verticalHeader()->hide();
    ui->tableView_ResStu->model()->setHeaderData(0,Qt::Horizontal,tr("Năm học"));
    ui->tableView_ResStu->model()->setHeaderData(1,Qt::Horizontal,tr("Mã môn học"));
    ui->tableView_ResStu->model()->setHeaderData(2,Qt::Horizontal,tr("Mã sinh viên"));
    ui->tableView_ResStu->model()->setHeaderData(3,Qt::Horizontal,tr("Họ tên"));
    ui->tableView_ResStu->model()->setHeaderData(4,Qt::Horizontal,tr("Lớp"));
    ui->tableView_ResStu->model()->setHeaderData(5,Qt::Horizontal,tr("Ngành học"));
    ui->tableView_ResStu->model()->setHeaderData(6,Qt::Horizontal,tr("Email"));
    ui->tableView_ResStu->model()->setHeaderData(7,Qt::Horizontal,tr("Giới tính"));
    ui->tableView_ResStu->model()->setHeaderData(8,Qt::Horizontal,tr("Số điện thoại"));
    ui->tableView_ResStu->model()->setHeaderData(9,Qt::Horizontal,tr("Căn cước công dân"));
    ui->tableView_ResStu->setColumnWidth(2,130);
    ui->tableView_ResStu->setColumnWidth(3,180);
    ui->tableView_ResStu->setColumnWidth(5,310);
    ui->tableView_ResStu->setColumnWidth(6,210);
    ui->tableView_ResStu->setColumnWidth(8,150);
    ui->tableView_ResStu->setColumnWidth(9,150);
}

void Teacher_Interactive_Interface::initTableListCourseOffer()
{
    tableListOffer->setTable("listCourseOffer");
    tableListOffer->setSort(0,Qt::AscendingOrder);
    tableListOffer->select();
    ui->tableView_listCourse->setModel(tableListOffer);
    ui->tableView_listCourse->verticalHeader()->hide();
    ui->tableView_listCourse->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_listCourse->model()->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    ui->tableView_listCourse->model()->setHeaderData(1,Qt::Horizontal,tr("Ngày bắt đầu"));
    ui->tableView_listCourse->model()->setHeaderData(2,Qt::Horizontal,tr("Ngày kết thúc"));
    ui->tableView_listCourse->model()->setHeaderData(3,Qt::Horizontal,tr("Ngày thi"));
    ui->tableView_listCourse->model()->setHeaderData(4,Qt::Horizontal,tr("Học kì"));
    ui->tableView_listCourse->model()->setHeaderData(5,Qt::Horizontal,tr("Số lượng"));
    ui->tableView_listCourse->model()->setHeaderData(7,Qt::Horizontal,tr("Số tín chỉ"));
    ui->tableView_listCourse->model()->setHeaderData(8,Qt::Horizontal,tr("Mã giảng viên"));
    ui->tableView_listCourse->setColumnWidth(1,125);
    ui->tableView_listCourse->setColumnWidth(2,125);
    ui->tableView_listCourse->setColumnWidth(3,125);
    ui->tableView_listCourse->setColumnWidth(8,125);
    ui->tableView_listCourse->hideColumn(6);
}

void Teacher_Interactive_Interface::initTableScheduleOffer()
{
    tableScheduleOffer->setTable("scheduleTableOffer");
    tableScheduleOffer->setSort(0,Qt::AscendingOrder);
    tableScheduleOffer->select();
    ui->tableView_ScheduleCourse->setModel(tableScheduleOffer);
    ui->tableView_ScheduleCourse->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_ScheduleCourse->verticalHeader()->hide();
    ui->tableView_ScheduleCourse->model()->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    ui->tableView_ScheduleCourse->model()->setHeaderData(1,Qt::Horizontal,tr("Ngày học"));
    ui->tableView_ScheduleCourse->model()->setHeaderData(2,Qt::Horizontal,tr("Phòng học"));
    ui->tableView_ScheduleCourse->model()->setHeaderData(3,Qt::Horizontal,tr("Ca học"));
    ui->tableView_ScheduleCourse->hideColumn(4);
    ui->tableView_ScheduleCourse->hideColumn(5);
}


void Teacher_Interactive_Interface::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void Teacher_Interactive_Interface::on_pushButton_3_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab_information);
}

void Teacher_Interactive_Interface::on_pushButton_2_clicked()
{
    this->indexx = 1;
    ui->tabWidget->insertTab(3,ui->tab_lichgiangday,"Lịch Giảng Dạy");
    ui->tabWidget->setCurrentWidget(ui->tab_lichgiangday);
    calendar->setCurrentPage(QDate::currentDate().year(),QDate::currentDate().month());
    clearTimeTable();
    pullOfData();
}

void Teacher_Interactive_Interface::on_pushButton_clicked()
{
    setIndexOfTab(1);
    ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);
    ui->label_8->hide();ui->label_9->hide();
    ui->comboBox_year->hide();ui->comboBox_class->hide();
}

void Teacher_Interactive_Interface::on_pushButton_4_clicked()
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

void Teacher_Interactive_Interface::on_pushButton_5_clicked()
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


void Teacher_Interactive_Interface::clearTimeTable()
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


void Teacher_Interactive_Interface::pullOfData()
{
    clearTimeTable();
    QSqlQuery query;
    query.prepare("SELECT MSGV FROM Lecturer_DATABASE WHERE USERNAME = '"+getUsername()+"'");
    query.exec();
    query.first();
    QString user_mssv = query.value(0).toString();
    QDate date;
    qDebug()<<"Major : "<<this->professional;
    for(int i = 1 ; i <= 2 ; ++i){
        qDebug()<<"INDXXXXXXXXXXXX-> "<<i;
        if (i == 1)
        {
            indexx = 1;
            query.prepare("SELECT detailTIMETABLE.courseCode,subjectID,dateStudy,studyTime,classroom FROM detailTIMETABLE,registered_course WHERE detailTIMETABLE.courseCode = registered_course.courseCode AND registered_course.subjectID = '"+this->professional+"' AND detailTIMETABLE.TeacherId ='"+user_mssv+"' ORDER BY dateStudy ASC");
        }
        if (i == 2)
        {
            indexx = 2;
            query.prepare("SELECT testSCHEDULE.courseCode,registered_course.subjectID,testSCHEDULE.dateTest,timeTest,testSCHEDULE.classroom FROM testSCHEDULE,registered_course WHERE testSCHEDULE.courseCode = registered_course.courseCode AND testSCHEDULE.TeacherId ='"+user_mssv+"' AND registered_course.subjectID = '"+this->professional+"' ORDER BY testSCHEDULE.dateTest ASC");
        }
        query.exec();
        while (query.next())
        {
            //monday
            date = QDate::fromString(query.value(2).toString(),"yyyy-MM-dd");
            if (date.toString("dd-MM-yyyy")==ui->label_day2->text() && query.value(3).toString() == "Sáng")
            {
                ui->label_m2->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_m2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_m2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day2->text() && query.value(3).toString() == "Trưa")
            {
                ui->label_n2->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_n2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_n2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }

            }
            if (date.toString("dd-MM-yyyy")==ui->label_day2->text() && query.value(3).toString() == "Tối")
            {
                ui->label_ni2->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_ni2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_ni2->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            //tuesday
            date = QDate::fromString(query.value(2).toString(),"yyyy-MM-dd");
            if (date.toString("dd-MM-yyyy")==ui->label_day3->text() && query.value(3).toString() == "Sáng")
            {
                ui->label_m3->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_m3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_m3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day3->text() && query.value(3).toString() == "Trưa")
            {
                ui->label_n3->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_n3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_n3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day3->text() && query.value(3).toString() == "Tối")
            {
                ui->label_ni3->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_ni3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_ni3->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            //wednesday
            date = QDate::fromString(query.value(2).toString(),"yyyy-MM-dd");
            if (date.toString("dd-MM-yyyy")==ui->label_day4->text() && query.value(3).toString() == "Sáng")
            {
                ui->label_m4->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_m4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_m4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day4->text() && query.value(3).toString() == "Trưa")
            {
                ui->label_n4->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_n4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_n4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day4->text() && query.value(3).toString() == "Tối")
            {
                ui->label_ni4->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_ni4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_ni4->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            //thursday
            date = QDate::fromString(query.value(2).toString(),"yyyy-MM-dd");
            if (date.toString("dd-MM-yyyy")==ui->label_day5->text() && query.value(3).toString() == "Sáng")
            {
                ui->label_m5->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_m5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_m5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day5->text() && query.value(3).toString() == "Trưa")
            {
                ui->label_n5->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_n5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_n5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day5->text() && query.value(3).toString() == "Tối")
            {
                ui->label_ni5->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_ni5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_ni5->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }

            //friday
            date = QDate::fromString(query.value(2).toString(),"yyyy-MM-dd");
            if (date.toString("dd-MM-yyyy")==ui->label_day6->text() && query.value(3).toString() == "Sáng")
            {
                ui->label_m6->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_m6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_m6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day6->text() && query.value(3).toString() == "Trưa")
            {
                ui->label_n6->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_n6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_n6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day6->text() && query.value(3).toString() == "Tối")
            {
                ui->label_ni6->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_ni6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_ni6->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }

            //saturday
            date = QDate::fromString(query.value(2).toString(),"yyyy-MM-dd");
            if (date.toString("dd-MM-yyyy")==ui->label_day7->text() && query.value(3).toString() == "Sáng")
            {
                ui->label_m7->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_m7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_m7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day7->text() && query.value(3).toString() == "Trưa")
            {
                ui->label_n7->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                //ui->label_n7->setToolTip(query.value(2).toString());
                if (indexx == 1)
                {
                    ui->label_n7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_n7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day7->text() && query.value(3).toString() == "Tối")
            {
                ui->label_ni7->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_ni7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_ni7->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }

            //sunday
            date = QDate::fromString(query.value(2).toString(),"yyyy-MM-dd");
            if (date.toString("dd-MM-yyyy")==ui->label_day8->text() && query.value(3).toString() == "Sáng")
            {
                ui->label_m8->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_m8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_m8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day8->text() && query.value(3).toString() == "Trưa")
            {
                ui->label_n8->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_n8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_n8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
            if (date.toString("dd-MM-yyyy")==ui->label_day8->text() && query.value(3).toString() == "Tối")
            {
                ui->label_ni8->setText("S : " + query.value(0).toString()+"\nP : "+query.value(4).toString());
                if (indexx == 1)
                {
                    ui->label_ni8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #a9fefe");
                }
                if (indexx == 2)
                {
                    ui->label_ni8->setStyleSheet("font-size : 14px;border : 1px groove #ffaa00;border-radius : 6px; background-color : #fdff9a");
                }
            }
        }
    }
}
// nếu là lịch học là 1 lịch thi là 2 đổi lại cho giáo viên thì chỉ có lịch giảng dạy thôi
void Teacher_Interactive_Interface::setIndex(int a){this->index = a;}
int Teacher_Interactive_Interface::getIndex(){return this->index;}

void Teacher_Interactive_Interface::on_pushButton_6_clicked()
{
    inittabletime(QDate::currentDate());
}

void Teacher_Interactive_Interface::inittabletime(QDate date_)
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
void Teacher_Interactive_Interface::on_pushButton_7_clicked()
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

void Teacher_Interactive_Interface::onSelectionChanged()
{
    QString x = calendar->selectedDate().toString("dd-MM-yyyy");
    QDate day;
    day = QDate::fromString(x,"dd-MM-yyyy");
    clearTimeTable();
    inittabletime(day);
    calendar->close();
}

void Teacher_Interactive_Interface::on_tabWidget_currentChanged(int index)
{
    if(index == 3){ui->label_18->hide();ui->tableView_ScheduleCourse->hide();ui->label_8->hide();ui->label_9->hide();ui->comboBox_class->hide();ui->comboBox_year->hide();ui->verticalSpacer_5->changeSize(20,490);}
    else if (index == 1)
    {
       setIndexOfTab(1);
       ui->label_18->hide();
       ui->tableView_ScheduleCourse->hide();
       ui->pushButton_9->show();
       ui->pushButton_10->show();
       ui->pushButton_11->show();
       ui->pushButton_12->show();
       ui->pushButton_13->show();
       ui->comboBox_op->hide();
       ui->radioButton_CT->hide();
       ui->radioButton_TC->hide();
       ui->comboBox_class->hide();
       ui->comboBox_year->hide();
       ui->label_8->hide();
       ui->label_9->hide();
       ui->verticalSpacer_5->changeSize(20,450);
       initComboBox();
    }
    else if(index == 2)
    {
        ui->label_18->hide();
        setIndexOfTab(2);
        ui->tableView_ScheduleCourse->hide();
        ui->pushButton_9->show();
        ui->pushButton_10->show();
        ui->pushButton_11->show();
        ui->pushButton_12->show();
        ui->pushButton_13->show();
        ui->comboBox_op->hide();
        ui->radioButton_CT->hide();
        ui->radioButton_TC->hide();
        ui->comboBox_class->hide();
        ui->comboBox_year->hide();
        ui->label_8->hide();
        ui->label_9->hide();
        ui->verticalSpacer_5->changeSize(20,450);
        initComboBoxTabDS();
        InitResStu();
    }
    else
    {
        ui->label_18->hide();
        ui->tableView_ScheduleCourse->hide();
        ui->pushButton_9->hide();
        ui->pushButton_10->hide();
        ui->pushButton_11->hide();
        ui->pushButton_12->hide();
        ui->pushButton_13->hide();
        ui->comboBox_op->hide();
        ui->radioButton_CT->hide();
        ui->radioButton_TC->hide();
        ui->label_9->hide();
        ui->label_8->hide();
        ui->comboBox_class->hide();
        ui->comboBox_year->hide();
        ui->verticalSpacer_5->changeSize(20,450);
    }
    if(index == 4){
        ui->tableView_ScheduleCourse->setFixedSize(400,350);
        ui->verticalSpacer_5->changeSize(20,42);
        ui->label_18->show();
        ui->tableView_ScheduleCourse->show();
    }
    if(ui->tabWidget->currentWidget() == ui->tab_information){
        ui->label_18->hide();
        ui->tableView_ScheduleCourse->hide();
        ui->pushButton_9->hide();
        ui->pushButton_10->hide();
        ui->pushButton_11->hide();
        ui->pushButton_12->hide();
        ui->pushButton_13->hide();
        ui->comboBox_op->hide();
        ui->radioButton_CT->hide();
        ui->radioButton_TC->hide();
        ui->label_9->hide();
        ui->label_8->hide();
        ui->comboBox_class->hide();
        ui->comboBox_year->hide();
        ui->verticalSpacer_5->changeSize(20,450);
    }
    if(ui->tabWidget->currentWidget() == ui->tab_quanlysinhvien){
        ui->label_18->hide();
        ui->tableView_ScheduleCourse->hide();
        ui->pushButton_9->show();
        ui->pushButton_10->show();
        ui->pushButton_11->show();
        ui->pushButton_12->show();
        ui->pushButton_13->show();
        ui->comboBox_op->hide();
        ui->radioButton_CT->hide();
        ui->radioButton_TC->hide();
        ui->comboBox_class->hide();
        ui->comboBox_year->hide();
        ui->label_8->hide();
        ui->label_9->hide();
        ui->verticalSpacer_5->changeSize(20,450);
        initComboBox();
    }
    if(ui->tabWidget->currentWidget() == ui->tab_resStudent){
        ui->label_18->hide();
        setIndexOfTab(2);
        ui->tableView_ScheduleCourse->hide();
        ui->pushButton_9->show();
        ui->pushButton_10->show();
        ui->pushButton_11->show();
        ui->pushButton_12->show();
        ui->pushButton_13->show();
        ui->comboBox_op->hide();
        ui->radioButton_CT->hide();
        ui->radioButton_TC->hide();
        ui->comboBox_class->hide();
        ui->comboBox_year->hide();
        ui->label_8->hide();
        ui->label_9->hide();
        ui->verticalSpacer_5->changeSize(20,450);
        initComboBoxTabDS();
        InitResStu();
    }
    if(ui->tabWidget->currentWidget() == ui->tab_lichgiangday){
        ui->label_18->hide();ui->tableView_ScheduleCourse->hide();ui->label_8->hide();ui->label_9->hide();ui->comboBox_class->hide();ui->comboBox_year->hide();ui->verticalSpacer_5->changeSize(20,490);
    }
    if(ui->tabWidget->currentWidget() == ui->tab_courseProposal){
        ui->tableView_ScheduleCourse->setFixedSize(400,350);
        ui->label_18->show();
        ui->verticalSpacer_5->changeSize(20,42);
        ui->tableView_ScheduleCourse->show();
        ui->label_18->show();
    }
}


void Teacher_Interactive_Interface::initTableMark()
{
    tableModel->setTable("MARK_OF_STUDENTS");
    tableModel->setFilter("MAMONHOC='"+this->professional+"'");
    tableModel->setSort(0,Qt::AscendingOrder);
    tableModel->select();
    ui->tableView->setModel(tableModel);

    ui->tableView->model()->setHeaderData(0,Qt::Horizontal,tr("Mã sinh viên"));
    ui->tableView->model()->setHeaderData(1,Qt::Horizontal,tr("Mã môn học"));
    ui->tableView->model()->setHeaderData(2,Qt::Horizontal,tr("Năm học"));
    ui->tableView->model()->setHeaderData(3,Qt::Horizontal,tr("Lớp"));
    ui->tableView->model()->setHeaderData(4,Qt::Horizontal,tr("Kiểm tra lần 1"));
    ui->tableView->model()->setHeaderData(5,Qt::Horizontal,tr("Kiểm tra lần 2"));
    ui->tableView->model()->setHeaderData(6,Qt::Horizontal,tr("Kiểm tra lần 3"));
    ui->tableView->model()->setHeaderData(7,Qt::Horizontal,tr("Điểm quá trình"));
    ui->tableView->model()->setHeaderData(8,Qt::Horizontal,tr("Điểm cuối kì"));
    ui->tableView->model()->setHeaderData(9,Qt::Horizontal,tr("Điểm tổng kết"));
    ui->tableView->model()->setHeaderData(10,Qt::Horizontal,tr("Thang điểm 4"));
    ui->tableView->model()->setHeaderData(11,Qt::Horizontal,tr("Điểm chữ"));
    ui->tableView->model()->setHeaderData(12,Qt::Horizontal,tr("Xếp loại"));
    ui->tableView->setColumnWidth(0,150);
    ui->tableView->setColumnWidth(1,120);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setColumnWidth(3,150);
    ui->tableView->setColumnWidth(4,150);
    ui->tableView->setColumnWidth(5,150);
    ui->tableView->setColumnWidth(6,120);
    ui->tableView->setColumnWidth(7,120);
    ui->tableView->setColumnWidth(8,120);
    ui->tableView->setColumnWidth(9,120);
    ui->tableView->setColumnWidth(10,120);
    ui->tableView->setColumnWidth(10,100);
    ui->tableView->setColumnWidth(10,100);
    ui->tableView->hideColumn(1);
}


void Teacher_Interactive_Interface::on_pushButton_11_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);

    changeMark->close();
    changeMark->clearData();
    changeMark->setIndex(1);
    changeMark->hideLineEdit();
    changeMark->hideClass();
    changeMark->setTextLineEdit("Cập nhật");
    changeMark->showComboBox();
    changeMark->setWindowTitle("Cập nhật điểm sinh viên");
    changeMark->show();
}

void Teacher_Interactive_Interface::initComboBox()
{
    ui->comboBox_year->clear();
    ui->comboBox_class->clear();
    ui->comboBox_op->clear();
    ui->label_9->setText("Lớp :");
    QSqlQuery qry,query,qury,qr;
    qry.prepare("SELECT DISTINCT SCHOOL_YEAR FROM MARK_OF_STUDENTS WHERE MAMONHOC='"+this->professional+"'");
    query.prepare("SELECT DISTINCT CLASSNAME FROM MARK_OF_STUDENTS WHERE MAMONHOC='"+this->professional+"'");
    qry.exec();
    query.exec();

    ui->comboBox_class->addItem("");
    ui->comboBox_year->addItem("");
    ui->comboBox_op->addItem("Mã sinh viên");
    ui->comboBox_op->addItem("Lớp");
    ui->comboBox_op->addItem("Năm học");
    ui->comboBox_op->addItem("Điểm cuối kì");
    ui->comboBox_op->addItem("Điểm tổng kết");
    ui->comboBox_op->addItem("Thang điểm 4");

    while (qry.next()) {
        ui->comboBox_year->addItem(qry.value(0).toString());
    }
    while (query.next()) {
        ui->comboBox_class->addItem(query.value(0).toString());
    }
}

void Teacher_Interactive_Interface::initComboBoxTabDS()
{
    ui->comboBox_class->clear();ui->comboBox_year->clear();ui->comboBox_op->clear();
    ui->label_9->setText("Mã học phần :");
    QSqlQuery qury,qry;
    qury.prepare("SELECT DISTINCT semester FROM registered_course WHERE registered_course.subjectID ='"+this->professional+"'");
    qry.prepare("SELECT DISTINCT courseCode FROM registered_course WHERE registered_course.subjectID ='"+this->professional+"'");
    qury.exec();qry.exec();
    ui->comboBox_class->addItem("");ui->comboBox_year->addItem("");
    while(qury.next()){
            ui->comboBox_year->addItem(qury.value(0).toString());
    }
    while (qry.next()) {
            ui->comboBox_class->addItem(qry.value(0).toString());
    }
}

void Teacher_Interactive_Interface::deleteRowCurrent()
{
    ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);
    bool check_K = false;
    QSqlQuery qury;
    QModelIndex in = ui->tableView->currentIndex();
    QString one = ui->tableView->model()->index(in.row(),0,QModelIndex()).data().toString();
    QString two = ui->tableView->model()->index(in.row(),2,QModelIndex()).data().toString();
    if (one.isEmpty() && two.isEmpty()) {
        check_K = true;
        messageBoxOneButton("Thông báo !","Hãy chọn một sinh viên nào đó để xóa !","Đồng ý",250,150,90,30);
    }
    QMessageBox mes;
    QPushButton* buttonYes,*buttonNo;
    if(!check_K)
    {
        mes.setStyleSheet("font : 8pt Courier");
        mes.setWindowTitle("Thông báo !");
        mes.setText("Bạn có chắc xóa điểm sinh viên này !");
        mes.setWindowIcon(QIcon(":/icon-file/waning.png"));
        buttonYes = mes.addButton(tr("Đồng ý"),QMessageBox::YesRole);
        buttonNo = mes.addButton(tr("Hủy"),QMessageBox::NoRole);
        buttonYes   ->setFixedSize(90,28);buttonNo->setFixedSize(90,28);
        buttonYes->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
        "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
        "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
        buttonNo->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
        "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
        "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
        buttonNo->setFocusPolicy(Qt::NoFocus);
        buttonYes->setFocusPolicy(Qt::NoFocus);
        mes.exec();
    }
    if(mes.clickedButton() == buttonYes)
    {
         qury.prepare("DELETE FROM MARK_OF_STUDENTS WHERE MSSV=:mssv AND SCHOOL_YEAR=:year AND MAMONHOC=:mamnon");
         qury.bindValue(0,one);
         qury.bindValue(1,two);
         qury.bindValue(2,this->professional);
         if(qury.exec()){messageBoxOneButton("Thông báo !","Xóa điểm sinh viên thành công !","Đồng ý",250,150,90,28);}
         tableModel->select();
    }
    if(mes.clickedButton() == buttonNo){}
}

void Teacher_Interactive_Interface::viewMark()
{
    QString de;
    QString year = ui->comboBox_year->currentText();
    QString class_ = ui->comboBox_class->currentText();
    ui->tableView->showColumn(2);ui->tableView->showColumn(3);
    if((year.isEmpty() && class_.isEmpty())){this->a = -1;de=QString("MAMONHOC='"+this->professional+"'");}
    if (year.isEmpty() && !class_.isEmpty()){this->a = 2;de=QString("CLASSNAME='"+class_+"' AND MAMONHOC='"+this->professional+"'");}
    if (!year.isEmpty() && class_.isEmpty()){this->a = 3;de=QString("SCHOOL_YEAR='"+year+"' AND MAMONHOC='"+this->professional+"'");}
    if (!year.isEmpty() && !class_.isEmpty()){this->a = 1;de=QString("SCHOOL_YEAR='"+year+"' AND CLASSNAME='"+class_+"' AND MAMONHOC='"+this->professional+"'");}
    tableModel->setFilter(de);
    tableModel->select();
    if (this->a == 2){ui->tableView->hideColumn(3);}
    if (this->a == 3){ui->tableView->hideColumn(2);}
    if(this->a == 1){ui->tableView->hideColumn(2);ui->tableView->hideColumn(3);}
    ui->tableView->hideColumn(1);
}

void Teacher_Interactive_Interface::updateInfo()
{
    ui->tabWidget->setCurrentWidget(ui->tab_information);
    QSqlQuery query,qry;
    query.prepare("SELECT * FROM Lecturer_DATABASE WHERE USERNAME = :USERNAME AND PASSWORD = :PASSWORD");
    query.bindValue(0,getUsername());
    query.bindValue(1,getPassword());
    query.exec();
    query.first();
    qry.prepare("SELECT _thuocKhoa.tenKhoa FROM _thuocKhoa WHERE _thuocKhoa.tenLop = :TENLOP ");
    qry.bindValue(0,query.value(2).toString());
    qry.exec();
    qry.first();
    if (getUsername() == query.value(9).toString() && getPassword() == query.value(10).toString())
    {
        QDate date_ = QDate::fromString(query.value(8).toString(),"yyyy-MM-dd");
        ui->label_name->setText("Họ tên : "+query.value(1).toString());
        ui->label_sex->setText("Giới tính : "+ query.value(7).toString());
        ui->label_phonenumber->setText("Số điện thoại : "+query.value(4).toString());
        ui->label_email->setText("Email : "+query.value(5).toString());
        ui->label_datebirth->setText("Ngày sinh : "+date_.toString("dd-MM-yyyy"));
        ui->label_identitycard->setText("Căn cước công dân : "+query.value(3).toString());
        ui->label_id->setText("Mã giáo viên : "+query.value(0).toString());
        ui->label_profess->setText("Môn dạy : "+query.value(2).toString());
        ui->label_address->setText("Địa chỉ : "+query.value(6).toString());
        if(!query.value(11).toString().isEmpty()){ui->label_ethnic->setText("Dân tộc : "+query.value(11).toString());}
        if(!query.value(12).toString().isEmpty()){ui->label_placeofbirth->setText("Nơi sinh : "+query.value(12).toString());}
        ui->label_khoa->setText("Khoa : "+qry.value(0).toString());
    }
    setPictureForUser();
}

void Teacher_Interactive_Interface::setPictureForUser()
{
    QSqlQuery queryy;
    QSqlQuery getStudentId;
    QString MSSV;
    getStudentId.prepare("SELECT MSGV FROM Lecturer_DATABASE WHERE USERNAME = :USERNAME");
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
        ui->label->setPixmap(pix);
    }
    else
    {
        QSqlQuery qry;
        QString SEX;
        qry.prepare("SELECT SEX FROM Lecturer_DATABASE WHERE USERNAME = :username");
        qry.bindValue(0,getUsername());
        qry.exec();
        qry.first();
        SEX = qry.value(0).toString();
        qDebug()<<"SEXXXXXXX"<<SEX;
        if ( SEX == "Nam")
        {
            //QImage image("qrc:/icon-file/boy_student.png");
            QPixmap pixmap(":/icon-file/boyy_student.png");
            ui->label->setPixmap(pixmap);
        }
        if (SEX == "Nữ")
        {
            QPixmap pixmap(":/icon-file/girll_student.png");
            ui->label->setPixmap(pixmap);
        }
    }
}

void Teacher_Interactive_Interface::setIndexOfTab(int h){this->indexOfTab = h;}

void Teacher_Interactive_Interface::closeEvent(QCloseEvent *event)
{
    if(checkClose){
        bool check = messageBoxTwoButton("Thông báo !","Bạn có chắc muốn thoát chương trình !","Đồng ý","Hủy",250,150,80,28);
        if(check){widget->close();changeMark->close();changeInfo->close();event->accept();qApp->exit();}else{event->ignore();}
    }
}

int Teacher_Interactive_Interface::printMarkOrList()
{
    widget->close();
    QMessageBox mes;
    mes.setWindowModality(Qt::WindowModal);mes.setWindowIcon(QIcon(":/icon-file/printer.png"));
    mes.setWindowTitle("In điểm và danh sách sinh viên");
    mes.setText("Hãy chọn lựa chọn tiếp theo của bạn ?");
    mes.setFixedSize(350,250);
    mes.setStyleSheet("font : 8pt Courier");
    QPushButton* buttonYes = mes.addButton(tr("In điểm"),QMessageBox::YesRole);
    QPushButton* buttonNo = mes.addButton(tr("DS sinh viên"),QMessageBox::YesRole);
    buttonYes->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
    buttonNo->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
    buttonYes->setFixedSize(150,28);
    buttonNo->setFixedSize(150,28);
    buttonYes->setIcon(QIcon(":/icon-file/result_pic_standard.png"));
    buttonNo->setIcon(QIcon(":/icon-file/student_icon.png"));
    buttonYes->setFocusPolicy(Qt::NoFocus);
    buttonNo->setFocusPolicy(Qt::NoFocus);
    mes.exec();
    if(mes.clickedButton() == buttonYes){mes.close();return 1;}
    if(mes.clickedButton() == buttonNo){mes.close();return -1;}
    return 2;
}

void Teacher_Interactive_Interface::doPrint()
{

    QXlsx::Document xlxs;QVariant a,b,c,d;int x,q,e,i,j,y;
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

    QString ys = comboxYear->currentText();QString cl = comboxClass->currentText();
    if(this->inPrint == 1){

               xlxs.mergeCells(CellRange("E3:G3"));
               xlxs.write("E3","DANH SÁCH ĐIỂM SINH VIÊN",format);
               xlxs.setRowHeight(3,30);
               xlxs.setRowHeight(6,20);

               xlxs.setColumnWidth(1,25);
               xlxs.setColumnWidth(2,20);
               xlxs.setColumnWidth(3,15);
               xlxs.setColumnWidth(4,20);
               xlxs.setColumnWidth(5,20);
               xlxs.setColumnWidth(6,20);
               xlxs.setColumnWidth(7,20);
               xlxs.setColumnWidth(8,28);
               xlxs.setColumnWidth(9,20);
               xlxs.setColumnWidth(10,20);
               xlxs.setColumnWidth(11,20);
               xlxs.setColumnWidth(12,15);
               xlxs.setColumnWidth(13,15);

               xlxs.write("A6","Mã sinh viên",formatCellHeader);
               xlxs.write("B6","Mã môn học",formatCellHeader);
               xlxs.write("C6","Năm học",formatCellHeader);
               xlxs.write("D6","Lớp",formatCellHeader);
               xlxs.write("E6","Kiểm tra lần 1",formatCellHeader);
               xlxs.write("F6","Kiểm tra lần 2",formatCellHeader);
               xlxs.write("G6","Kiểm tra lần 3",formatCellHeader);
               xlxs.write("H6","Điểm thường xuyên",formatCellHeader);
               xlxs.write("I6","Điểm cuối kì",formatCellHeader);
               xlxs.write("J6","Điểm tổng kết",formatCellHeader);
               xlxs.write("K6","Thang điểm 4",formatCellHeader);
               xlxs.write("L6","Điểm chữ",formatCellHeader);
               xlxs.write("M6","Xếp loại",formatCellHeader);

               x = ui->tableView->model()->rowCount();y = ui->tableView->model()->columnCount();
               q = 5,e = 0;
               for (i = 0 ;i < x ; ++i) {
                   b = ui->tableView->model()->index(i,2,QModelIndex()).data().toString();
                   c = ui->tableView->model()->index(i,1,QModelIndex()).data().toString();
                   d = ui->tableView->model()->index(i,3,QModelIndex()).data().toString();

                   if(b == ys && c == this->professional && d == cl){
                       ++q;
                        for (j = 0;j < y ;++j ) {
                            a = ui->tableView->model()->index(i,j,QModelIndex()).data().toString();
                            xlxs.write(q+1,j+1,a,formatCell);
                            qDebug()<<i<<" "<<" "<<j<<" "<<a<<" "<<""<<q<<" "<<e<<" "<<this->professional;
                      }
                            xlxs.setRowHeight(q+1,15);
                   }
               }

               QString setFill = QString("Excel (*.xlsx)");
               QString des = QFileDialog::getSaveFileName(this,"Lưu file","",setFill);
               QString pathName = des;
               bool check = xlxs.saveAs(des);
               if(check){widget->close();messageBoxOneButton("Thông báo !","Xuất file điểm thành công !","Đồng ý",250,150,80,28);}
               else{widget->close();messageBoxOneButton("Thông báo !","Xuất file điểm thất bại, thử lại sau !","Đồng ý",250,150,80,28);}
    }

    if(this->inPrint == -1){

        xlxs.mergeCells(CellRange("D3:H3"));
        xlxs.write("D3","DANH SÁCH SINH VIÊN ĐĂNG KÍ HỌC PHẦN",format);
        xlxs.setRowHeight(3,30);
        xlxs.setRowHeight(6,20);

        xlxs.setColumnWidth(1,15);
        xlxs.setColumnWidth(2,15);
        xlxs.setColumnWidth(3,20);
        xlxs.setColumnWidth(4,25);
        xlxs.setColumnWidth(5,15);
        xlxs.setColumnWidth(6,40);
        xlxs.setColumnWidth(7,30);
        xlxs.setColumnWidth(8,15);
        xlxs.setColumnWidth(9,20);
        xlxs.setColumnWidth(10,20);


        xlxs.write("A6","Năm học",formatCellHeader);
        xlxs.write("B6","Mã môn học",formatCellHeader);
        xlxs.write("C6","Mã sinh viên",formatCellHeader);
        xlxs.write("D6","Họ tên",formatCellHeader);
        xlxs.write("E6","Lớp",formatCellHeader);
        xlxs.write("F6","Ngành học",formatCellHeader);
        xlxs.write("G6","Email",formatCellHeader);
        xlxs.write("H6","Giới tính",formatCellHeader);
        xlxs.write("I6","Số điện thoại",formatCellHeader);
        xlxs.write("J6","Căn cước công dân",formatCellHeader);

        x= ui->tableView_ResStu->model()->rowCount();y = ui->tableView_ResStu->model()->columnCount();
        q = 5,e=0;
        for (i = 0;i < x ;++i ) {
            a = ui->tableView_ResStu->model()->index(i,0,QModelIndex()).data().toString();
            b = ui->tableView_ResStu->model()->index(i,1,QModelIndex()).data().toString();
            qDebug()<<a<<" "<<b<<" "<<ys<<" "<<cl;
            if(a == ys && b == cl){
                ++q;
                for (j = 0;j < y ;++j ) {
                    c = ui->tableView_ResStu->model()->index(i,j,QModelIndex()).data().toString();
                    xlxs.write(q+1,j+1,c,formatCell);
                    qDebug()<<i<<" "<<" "<<j<<" "<<c<<" "<<""<<q<<" "<<e<<" "<<this->professional;
                }
                    xlxs.setRowHeight(q+1,15);
            }
        }

        QString setFill = QString("Excel (*.xlsx)");
        QString des = QFileDialog::getSaveFileName(this,"Lưu file","",setFill);
        QString pathName = des;
        qDebug()<<"Location - >> : "<<des<<" Mjot"<<this->professional;

        bool check = xlxs.saveAs(des);

        if(check){widget->close();messageBoxOneButton("Thông báo !","Xuất danh sách sinh viên thành công !","Đồng ý",250,150,80,28);}
        else{widget->close();messageBoxOneButton("Thông báo !","Xuất danh sách điểm thất bại, thử lại sau !","Đồng ý",250,150,80,28);}
    }
}

void Teacher_Interactive_Interface::doWhenComboBoxChanged(QString arf)
{
    comboxYear->clear();
    QSqlQuery qury;
    if(this->inPrint == 1){
        qury.prepare("SELECT DISTINCT SCHOOL_YEAR FROM MARK_OF_STUDENTS WHERE CLASSNAME='"+arf+"' AND MAMONHOC='"+this->professional+"'");
        qury.exec();
        while(qury.next()){comboxYear->addItem(qury.value(0).toString());}
    }
    if(this->inPrint == -1){
        qury.prepare("SELECT DISTINCT semester FROM registered_course WHERE courseCode='"+arf+"' AND subjectID='"+this->professional+"'");
        qury.exec();
        while(qury.next()){comboxYear->addItem(qury.value(0).toString());}
    }
}

int Teacher_Interactive_Interface::doWhenHaveDoubleClickOnTableViewMark()
{
    QMessageBox mes;
    mes.setWindowModality(Qt::WindowModal);mes.setWindowIcon(QIcon(":/icon-file/more_options_standard.png"));
    mes.setWindowTitle("Lựa chọn của bạn là gì ?");
    mes.setText("Hãy chọn lựa chọn tiếp theo của bạn ?");
    mes.setFixedSize(350,250);
    mes.setStyleSheet("font : 8pt Courier");
    QPushButton* buttonYes = mes.addButton(tr("Cập nhật"),QMessageBox::YesRole);
    QPushButton* buttonNo = mes.addButton(tr("Xóa"),QMessageBox::YesRole);
    mes.setWindowRole("NoRoler");
    buttonYes->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
    buttonNo->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
    buttonYes->setFixedSize(150,28);
    buttonNo->setFixedSize(150,28);
    buttonYes->setIcon(QIcon(":/icon-file/result_pic_standard.png"));
    buttonNo->setIcon(QIcon(":/icon-file/student_icon.png"));
    buttonYes->setFocusPolicy(Qt::NoFocus);
    buttonNo->setFocusPolicy(Qt::NoFocus);
    mes.exec();
    if(mes.clickedButton() == buttonYes){mes.close();return 2;}
    if(mes.clickedButton() == buttonNo){mes.close();return -2;}
    return 3;
}

void Teacher_Interactive_Interface::doWhenUiComboBoxYearChange(QString ql)
{
    QSqlQuery qury;
    ui->comboBox_class->clear();
    if(getIndexOfTab() == 1 && !ql.isEmpty()){
        qury.prepare("SELECT DISTINCT CLASSNAME FROM MARK_OF_STUDENTS WHERE MAMONHOC='"+this->professional+"' AND SCHOOL_YEAR='"+ql+"'");
        qury.exec();
        ui->comboBox_class->addItem("");
        while(qury.next()){
            ui->comboBox_class->addItem(qury.value(0).toString());
        }
    }
    if(getIndexOfTab() == 2 && !ql.isEmpty()){
        qury.prepare("SELECT DISTINCT courseCode FROM registered_course WHERE semester='"+ql+"' AND subjectID = '"+this->professional+"'");
        qury.exec();
        ui->comboBox_class->addItem("");
        while(qury.next()){
            ui->comboBox_class->addItem(qury.value(0).toString());
        }
    }


    if(getIndexOfTab() == 1 && ql.isEmpty()){
        qury.prepare("SELECT DISTINCT CLASSNAME FROM MARK_OF_STUDENTS WHERE MAMONHOC='"+this->professional+"'");
        qury.exec();
        ui->comboBox_class->addItem("");
        while(qury.next()){
            ui->comboBox_class->addItem(qury.value(0).toString());
        }
    }
    if(getIndexOfTab() == 2 && ql.isEmpty()){
        qury.prepare("SELECT DISTINCT courseCode FROM registered_course WHERE subjectID = '"+this->professional+"'");
        qury.exec();
        ui->comboBox_class->addItem("");
        while(qury.next()){
            ui->comboBox_class->addItem(qury.value(0).toString());
        }
    }
}

void Teacher_Interactive_Interface::keyReleaseEvent(QKeyEvent *event)
{
    qDebug()<<"F5 it's run";
    if(event->key() == Qt::Key_F5 && ui->tabWidget->currentIndex() == 2){initTableRes();}
}

bool Teacher_Interactive_Interface::checkCourseIdIsValid()
{
    QString a = ui->lineEdit_courseId->text();
    QRegularExpression rec("[0-9]",QRegularExpression::CaseInsensitiveOption);
    QString id = ui->lineEdit_courseId->text();
    QRegularExpressionMatch rec_math = rec.match(id);
    bool it = rec_math.hasMatch();
    if(it){
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkCourseIdIsExist()
{
    QSqlQuery qury;
    qury.prepare("SELECT courseID FROM listCourseOffer WHERE subjectID='"+this->professional+"' AND courseID='"+ui->lineEdit_courseId->text()+"'");
    qury.exec();
    qury.first();
    if(qury.value(0).toString() == ui->lineEdit_courseId->text()){
        ui->lineEdit_courseId->setStyleSheet("color : red");
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkDateStartIsValid()
{
    string dater= ui->lineEdit_dateStart->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_dateStart->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_dateStart->text(),"dd-MM-yyyy");
    if (!date.isValid())
    {
        return false;
    }
    if (date.isValid())
    {
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkDateEndIsValid()
{
    string dater= ui->lineEdit_dateEnd->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_dateEnd->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_dateEnd->text(),"dd-MM-yyyy");
    if (!date.isValid())
    {
        return false;
    }
    if (date.isValid())
    {
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkDateTestIsValid()
{
    string dater= ui->lineEdit_dateTest->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_dateTest->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_dateTest->text(),"dd-MM-yyyy");
    if (!date.isValid())
    {
        return false;
    }
    if (date.isValid())
    {
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkSemesterIsValid()
{
    QString a = ui->lineEdit_semester->text();
    if(a.toInt() >= QDate::currentDate().year()){
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkLimitedQuantityIsValid()
{
    int a =  ui->lineEdit_quantity->text().toInt();
    if(a > 0){
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkDateStudyIsValid()
{
    string dater= ui->lineEdit_dateStu->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_dateStu->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_dateStu->text(),"dd-MM-yyyy");
    if (!date.isValid())
    {
        return false;
    }
    if (date.isValid())
    {
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkRoomIsValid()
{
    QRegularExpression rec("[A-Z]{1}[0-9]",QRegularExpression::CaseInsensitiveOption);
    QString cla = ui->lineEdit_class->text();
    QRegularExpressionMatch rec_math = rec.match(cla);
    if(rec_math.hasMatch()){
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkAllDateIsValid()
{
    QDate dateS,dateE,dateT;
    dateS=QDate::fromString(ui->lineEdit_dateStart->text(),"dd-MM-yyyy");
    dateE=QDate::fromString(ui->lineEdit_dateEnd->text(),"dd-MM-yyyy");
    dateT=QDate::fromString(ui->lineEdit_dateTest->text(),"dd-MM-yyyy");
    if(dateS < dateE && dateT > dateE ){
        ui->lineEdit_dateStart->setStyleSheet("color : black");
        ui->lineEdit_dateEnd->setStyleSheet("color : black");
        ui->lineEdit_dateTest->setStyleSheet("color : black");
        return true;
    }
    ui->lineEdit_dateStart->setStyleSheet("color : red");
    ui->lineEdit_dateEnd->setStyleSheet("color : red");
    ui->lineEdit_dateTest->setStyleSheet("color : red");
    return false;
}

bool Teacher_Interactive_Interface::checkDateStuValid()
{
    QSqlQuery qury;
    qury.prepare("SELECT * FROM listCourseOffer WHERE semester = '"+ui->comboBox_ye->currentText()+"' AND courseID = '"+ui->comboBox_courseId->currentText()+"' AND subjectID='"+this->professional+"'");
    qury.exec();
    qury.first();
    QDate dateS,dateE,dateStu;QString d;
    dateS=QDate::fromString(qury.value(1).toString(),"yyyy-MM-dd");
    dateE=QDate::fromString(qury.value(2).toString(),"yyyy-MM-dd");
    dateStu=QDate::fromString(ui->lineEdit_dateStu->text(),"dd-MM-yyyy");
    d = dateStu.toString("yyyy-MM-dd");
    dateStu = QDate::fromString(d,"yyyy-MM-dd");
    qDebug()<<"DATESSS"<<dateS<<" "<<dateE<<" "<<dateStu;
    if(dateStu >= dateS && dateStu < dateE){ui->lineEdit_dateStu->setStyleSheet("color : black");return true;}
    ui->lineEdit_dateStu->setStyleSheet("color : red");
    return false;
}

bool Teacher_Interactive_Interface::checkSemesterIsChecked()
{
    if(ui->radioButton_sang->isChecked() || ui->radioButton_trua->isChecked() || ui->radioButton_toi->isChecked()){
        return true;
    }
    return false;
}

bool Teacher_Interactive_Interface::checkNumresIsValid()
{
    int n = ui->lineEdit_numRes->text().toInt();
    if(n >0 && n <= 9){
        return true;
    }
    return false;
}

void Teacher_Interactive_Interface::insertCourseIntoDatabase()
{
    QString id,se,nu,date_inS,date_inE,date_inT;
    id = ui->lineEdit_courseId->text();
    se = ui->lineEdit_semester->text();
    nu = ui->lineEdit_quantity->text();

    QDate dateS = QDate::fromString(ui->lineEdit_dateStart->text(),"dd-MM-yyyy");
    QDate dateE = QDate::fromString(ui->lineEdit_dateEnd->text(),"dd-MM-yyyy");
    QDate dateT= QDate::fromString(ui->lineEdit_dateTest->text(),"dd-MM-yyyy");
    date_inS = dateS.toString("yyyy-MM-dd");
    date_inE = dateE.toString("yyyy-MM-dd");
    date_inT = dateT.toString("yyyy-MM-dd");
    qDebug()<<id<<" "<<date_inS<<" "<<date_inE<<" "<<date_inT<<" "<<se<<" "<<nu<<" "<<this->professional;
    QSqlQuery qury;
    qury.prepare("INSERT INTO listCourseOffer(courseID,dateStart,dateEnd,dateTest,semester,limitedQuantity,subjectID,numRes,TEACHERID)"
                 "VALUES('"+id+"',CAST('"+date_inS+"' AS DATE),CAST('"+date_inE+"' AS DATE),CAST('"+date_inT+"' AS DATE),'"+se+"','"+nu+"','"+this->professional+"','"+ui->lineEdit_numRes->text()+"','"+msgv+"')");
    qury.exec();
}

void Teacher_Interactive_Interface::insertScheduleOfferDatabase()
{
    QString dateStuIn,room,id,ses,y;QSqlQuery qury;
    QDate dateStu = QDate::fromString(ui->lineEdit_dateStu->text(),"dd-MM-yyyy");
    dateStuIn = dateStu.toString("yyyy-MM-dd");
    room = ui->lineEdit_class->text();
    id = ui->comboBox_courseId->currentText();
    y = ui->comboBox_ye->currentText();
    if(ui->radioButton_sang->isChecked()){ses = ui->radioButton_sang->text();}
    if(ui->radioButton_trua->isChecked()){ses = ui->radioButton_trua->text();}
    if(ui->radioButton_toi->isChecked()){ses = ui->radioButton_toi->text();}
    qury.prepare("INSERT INTO scheduleTableOffer(courseId,dateStudy,room,session,subjectID,semester,teacherID)"
                 "VALUES('"+id+"',CAST('"+dateStuIn+"' AS DATE),'"+room+"',N'"+ses+"','"+this->professional+"','"+y+"','"+msgv+"');");
    qury.exec();
}

void Teacher_Interactive_Interface::whenYearOfCourseChange()
{
    ui->comboBox_courseId->clear();
    QSqlQuery qry;
    qry.prepare("SELECT DISTINCT courseID FROM listCourseOffer WHERE subjectID ='"+this->professional+"' AND semester ='"+ui->comboBox_ye->currentText()+"' ORDER BY courseID DESC");
    qry.exec();
    while(qry.next()){
        ui->comboBox_courseId->addItem(qry.value(0).toString());
    }
}

QString Teacher_Interactive_Interface::checkDateStudyIsExist()
{
    QSqlQuery qury,qry;QString dateStuIn,ses;
    QDate dateStu = QDate::fromString(ui->lineEdit_dateStu->text(),"dd-MM-yyyy");
    dateStuIn = dateStu.toString("yyyy-MM-dd");
    if(ui->radioButton_sang->isChecked()){ses = ui->radioButton_sang->text();}
    if(ui->radioButton_trua->isChecked()){ses = ui->radioButton_trua->text();}
    if(ui->radioButton_toi->isChecked()){ses = ui->radioButton_toi->text();}
    qury.prepare("SELECT * FROM scheduleTableOffer WHERE courseId='"+ui->comboBox_courseId->currentText()+"' AND dateStudy='"+dateStuIn+"' AND ROOM='"+ui->lineEdit_class->text()+"' AND session=N'"+ses+"' AND subjectID='"+this->professional+"' AND semester='"+ui->comboBox_ye->currentText()+"'");
    qury.exec();
    qury.first();
    qDebug()<<"INITAL - 1> "<<ui->comboBox_courseId->currentText()<<" "<<ui->comboBox_ye->currentText()<<" "<<dateStuIn<<" "<<ui->lineEdit_class->text()<<" "<<ses;
    qDebug()<<"EMILYYYYY 1-> "<<qury.value(0).toString()<<qury.value(1).toString()<<qury.value(2).toString()<<qury.value(3).toString()<<qury.value(4).toString()<<qury.value(5).toString();
    if(ses == qury.value(3).toString()){qDebug()<<"EQUAL1";}else{qDebug()<<"NOT EQUAL1";}
    if(qury.value(0).toString() == ui->comboBox_courseId->currentText() && qury.value(1).toString() == dateStuIn && qury.value(2).toString() == ui->lineEdit_class->text() && qury.value(3).toString() == ses && qury.value(4).toString() == this->professional && qury.value(5).toString() == ui->comboBox_ye->currentText()){
        ui->lineEdit_class->setStyleSheet("color : red");
        ui->lineEdit_dateStu->setStyleSheet("color : red");
        ui->comboBox_courseId->setStyleSheet("color : red");
        ui->comboBox_ye->setStyleSheet("color : red");
        return "all";
    }

    dateStu = QDate::fromString(ui->lineEdit_dateStu->text(),"dd-MM-yyyy");
    dateStuIn = dateStu.toString("yyyy-MM-dd");
    if(ui->radioButton_sang->isChecked()){ses = ui->radioButton_sang->text();}
    if(ui->radioButton_trua->isChecked()){ses = ui->radioButton_trua->text();}
    if(ui->radioButton_toi->isChecked()){ses = ui->radioButton_toi->text();}
    qry.prepare("SELECT * FROM scheduleTableOffer WHERE dateStudy='"+dateStuIn+"' AND ROOM='"+ui->lineEdit_class->text()+"' AND session=N'"+ses+"' AND subjectID='"+this->professional+"' AND semester='"+ui->comboBox_ye->currentText()+"'");
    qry.exec();
    qry.first();

    qDebug()<<"INITAL - 2> "<<dateStuIn<<" "<<ui->lineEdit_class->text()<<" "<<ses<<" "<<this->professional<<" "<<ui->comboBox_ye->currentText();
    qDebug()<<"EMILYYYYY 2-> "<<qry.value(1).toString()<<qry.value(2).toString()<<qry.value(3).toString()<<qry.value(4).toString()<<qry.value(5).toString();
    if(qry.value(1).toString() == dateStuIn ){qDebug()<<"date 1 true";}else{qDebug()<<"date 1 false";}
    if(qry.value(2).toString() == ui->lineEdit_class->text() ){qDebug()<<"class 2 true";}else{qDebug()<<" class 2 false";}
    if(qry.value(3).toString() == ses ){qDebug()<<"session 3 true";}else{qDebug()<<"session 3 false";}
    if(qry.value(4).toString() == this->professional ){qDebug()<<"pro 4 true";}else{qDebug()<<"pro 4 false";}
    if(qry.value(5).toString() == ui->comboBox_ye->currentText() ){qDebug()<<"semester 5 true";}else{qDebug()<<"semester 5 false";}

    if(ses == qry.value(3).toString()){qDebug()<<"EQUAL2";}else{qDebug()<<"NOT EQUAL2";}
    if(qry.value(1).toString() == dateStuIn && qry.value(2).toString() == ui->lineEdit_class->text() && qry.value(3).toString() == ses && qry.value(4).toString() == this->professional && qry.value(5).toString() == ui->comboBox_ye->currentText()){
        ui->lineEdit_class->setStyleSheet("color : red");
        ui->lineEdit_dateStu->setStyleSheet("color : red");
        return "al";
    }
    ui->lineEdit_class->setStyleSheet("color : black");
    ui->lineEdit_dateStu->setStyleSheet("color : black");
    ui->comboBox_courseId->setStyleSheet("color : black");
    ui->comboBox_ye->setStyleSheet("color : black");
    return "a";
}


int Teacher_Interactive_Interface::getIndexOfTab(){return this->indexOfTab;}

void Teacher_Interactive_Interface::on_pushButton_10_clicked()
{

    ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);

    changeMark->close();
    changeMark->clearData();
    changeMark->setIndex(2);
    changeMark->hideComboBox();
    changeMark->showLineEdit();
    changeMark->showClass();
    changeMark->setTextLineEdit("Thêm");
    changeMark->setWindowTitle("Thêm điểm sinh viên");
    changeMark->show();
}

void Teacher_Interactive_Interface::on_tableView_doubleClicked(const QModelIndex &index)
{
    xRe = doWhenHaveDoubleClickOnTableViewMark();qDebug()<<"X SLECTION -> "<<xRe;
    if(xRe == -2){xRe=3;deleteRowCurrent();initComboBox();}
    if(xRe == 2){
        changeMark->clearData();
        changeMark->setIndex(1);
        changeMark->close();
        setIndex(1);
        QString mssv = ui->tableView->model()->index(index.row(),0,QModelIndex()).data().toString();
        QString year,classname,lan1,lan2,lan3,thuongxuyen,cuoiki,tongket,thangdiem4,diemchu,xeploai;
        if(ui->comboBox_year->currentText().isEmpty()){
            year = ui->tableView->model()->index(index.row(),2).data().toString();
        }
        if(!ui->comboBox_year->currentText().isEmpty()){
            year = ui->comboBox_year->currentText();
        }
        if(ui->comboBox_class->currentText().isEmpty()){
            classname = ui->tableView->model()->index(index.row(),3).data().toString();
        }
        if(!ui->comboBox_class->currentText().isEmpty()){
            classname = ui->comboBox_class->currentText();
        }

        lan1 = ui->tableView->model()->index(index.row(),4).data().toString();
        lan2 = ui->tableView->model()->index(index.row(),5).data().toString();
        lan3 = ui->tableView->model()->index(index.row(),6).data().toString();
        thuongxuyen = ui->tableView->model()->index(index.row(),7).data().toString();
        cuoiki = ui->tableView->model()->index(index.row(),8).data().toString();
        tongket = ui->tableView->model()->index(index.row(),9).data().toString();
        thangdiem4 = ui->tableView->model()->index(index.row(),10).data().toString();
        diemchu = ui->tableView->model()->index(index.row(),11).data().toString();
        xeploai = ui->tableView->model()->index(index.row(),12).data().toString();

        changeMark->setStudentMark(mssv,year,lan1,lan2,lan3,thuongxuyen,cuoiki,tongket,thangdiem4,diemchu,xeploai);

        if (getIndex() == 1) {
            changeMark->setIndex(1);
            changeMark->setTextLineEdit("Cập nhật");
            changeMark->hideLineEdit();
            changeMark->hideClass();
            changeMark->showComboBox();
            changeMark->show();
        }
        xRe = 3;
    }
}


void Teacher_Interactive_Interface::on_pushButton_9_clicked()
{
    deleteRowCurrent();
    initComboBox();
}


void Teacher_Interactive_Interface::on_pushButton_13_clicked()
{
    if(getIndexOfTab() == 1)
    {
        ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);
        ui->label_8->show();
        ui->label_9->show();
        ui->comboBox_year->show();
        ui->comboBox_class->show();
        ui->comboBox_op->show();
        ui->radioButton_CT->show();
        ui->radioButton_TC->show();
        ui->horizontalSpacer_13->changeSize(20,10);
    }
    if(getIndexOfTab() == 2)
    {
        ui->tabWidget->setCurrentWidget(ui->tab_resStudent);
        ui->label_8->show();
        ui->label_9->show();
        ui->comboBox_year->show();
        ui->comboBox_class->show();
        ui->comboBox_op->hide();
        ui->radioButton_CT->hide();
        ui->radioButton_TC->hide();
        ui->horizontalSpacer_13->changeSize(10,10);
    }
}

void Teacher_Interactive_Interface::tickChange(int index)
{
    if (index == 0 && getIndexOfTab() == 1){
        if (ui->radioButton_TC->isChecked()) {tableModel->setSort(0,Qt::AscendingOrder);tableModel->select();}
        if(ui->radioButton_CT->isChecked()){tableModel->setSort(0,Qt::DescendingOrder);tableModel->select();}
    }
    if(index == 1 && getIndexOfTab() == 1){
        if (ui->radioButton_TC->isChecked()) {tableModel->setSort(3,Qt::AscendingOrder);tableModel->select();}
        if(ui->radioButton_CT->isChecked()){tableModel->setSort(3,Qt::DescendingOrder);tableModel->select();}
    }
    if(index == 2 && getIndexOfTab() == 1){
        if (ui->radioButton_TC->isChecked()) {tableModel->setSort(2,Qt::AscendingOrder);tableModel->select();}
        if(ui->radioButton_CT->isChecked()){tableModel->setSort(2,Qt::DescendingOrder);tableModel->select();}
    }
    if(index == 3 && getIndexOfTab() == 1){
        if (ui->radioButton_TC->isChecked()) {tableModel->setSort(8,Qt::AscendingOrder);tableModel->select();}
        if(ui->radioButton_CT->isChecked()){tableModel->setSort(8,Qt::DescendingOrder);tableModel->select();}
    }
    if(index == 4 && getIndexOfTab() == 1){
        if (ui->radioButton_TC->isChecked()) {tableModel->setSort(9,Qt::AscendingOrder);tableModel->select();}
        if(ui->radioButton_CT->isChecked()){tableModel->setSort(9,Qt::DescendingOrder);tableModel->select();}
    }
    if(index == 5 && getIndexOfTab() == 1){
        if (ui->radioButton_TC->isChecked()) {tableModel->setSort(10,Qt::AscendingOrder);tableModel->select();}
        if(ui->radioButton_CT->isChecked()){tableModel->setSort(10,Qt::DescendingOrder);tableModel->select();}
    }
}

void Teacher_Interactive_Interface::on_comboBox_op_currentIndexChanged(int index)
{
    tickChange(index);
}

void Teacher_Interactive_Interface::on_radioButton_TC_clicked()
{
    tickChange(ui->comboBox_op->currentIndex());
}

void Teacher_Interactive_Interface::on_radioButton_CT_clicked()
{
    tickChange(ui->comboBox_op->currentIndex());
}

void Teacher_Interactive_Interface::on_comboBox_year_currentIndexChanged(int index)
{
    if(getIndexOfTab() == 1)
    {
        viewMark();
    }
    if(getIndexOfTab() == 2)
    {
        InitResStu();
    }
}

void Teacher_Interactive_Interface::on_comboBox_class_currentIndexChanged(int index)
{
    if(getIndexOfTab() == 1)
    {
        viewMark();
    }
    if(getIndexOfTab() == 2)
    {
        InitResStu();
    }
}

void Teacher_Interactive_Interface::showText()
{
    ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);
    tableModel->select();
    initComboBox();
}

void Teacher_Interactive_Interface::on_pushButton_14_clicked()
{
    changeInfo->close();
    changeInfo->getUserPass(getUsername(),getPassword());
    changeInfo->setDistinguish(2);
    changeInfo->show();
}

void Teacher_Interactive_Interface::on_actionTh_ng_tin_c_c_nh_n_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_information);
}

void Teacher_Interactive_Interface::on_actionC_p_nh_t_th_ng_tin_c_nh_n_triggered()
{
    changeInfo->close();
    changeInfo->getUserPass(getUsername(),getPassword());
    changeInfo->setDistinguish(2);
    changeInfo->show();
}

void Teacher_Interactive_Interface::on_actionQu_n_l_sinh_vi_n_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);
}

void Teacher_Interactive_Interface::on_actionL_ch_gi_ng_d_y_triggered()
{
    this->indexx = 1;
    ui->tabWidget->setCurrentWidget(ui->tab_lichgiangday);
    calendar->setCurrentPage(QDate::currentDate().year(),QDate::currentDate().month());
    inittabletime(QDate::currentDate());
    pullOfData();
}

void Teacher_Interactive_Interface::on_actionTh_m_i_m_cho_sinh_vi_n_triggered()
{
    changeMark->close();
    changeMark->clearData();
    changeMark->setIndex(2);
    changeMark->hideComboBox();
    changeMark->showLineEdit();
    changeMark->showClass();
    changeMark->setTextLineEdit("Thêm");
    changeMark->setWindowTitle("Thêm điểm sinh viên");
    ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);
    changeMark->show();
}

void Teacher_Interactive_Interface::on_actionX_a_i_m_sinh_vi_n_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_resStudent);
    deleteRowCurrent();
    initComboBox();
}

void Teacher_Interactive_Interface::on_actionCh_nh_s_a_i_m_triggered()
{
    changeMark->close();
    changeMark->clearData();
    changeMark->setIndex(1);
    changeMark->hideLineEdit();
    changeMark->hideClass();
    changeMark->setTextLineEdit("Cập nhật");
    changeMark->showComboBox();
    changeMark->setWindowTitle("Cập nhật điểm sinh viên");
    ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);
    changeMark->show();
}

void Teacher_Interactive_Interface::on_action_i_m_t_kh_u_triggered()
{
    changePW->close();
    changePW->setNum(2);
    changePW->show();
}

void Teacher_Interactive_Interface::on_actionTho_t_triggered()
{
    this->close();
}


void Teacher_Interactive_Interface::on_actionDanh_s_ch_sinh_vi_n_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_resStudent);
}

void Teacher_Interactive_Interface::on_actionL_ch_thi_triggered()
{
    this->indexx = 2;
    ui->tabWidget->setCurrentWidget(ui->tab_lichgiangday);
    calendar->setCurrentPage(QDate::currentDate().year(),QDate::currentDate().month());
    inittabletime(QDate::currentDate());
    pullOfData();
}

void Teacher_Interactive_Interface::setTime()
{
    ui->label_10->setText(QDate::currentDate().toString() +"  "+ QTime::currentTime().toString("hh:mm:ss"));
}

void Teacher_Interactive_Interface::on_action_ng_xu_t_triggered()
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

void Teacher_Interactive_Interface::on_pushButton_12_clicked()
{
    this->inPrint = printMarkOrList();
    widget->setWindowTitle("Lựa chọn đi nào !");
    widget->setWindowIcon(QIcon(":/more_options_standard.png"));
    QSqlQuery qry,query;
    labelYear->setFixedSize(70,28);labelClass->setFixedSize(70,28);
    labelYear->setStyleSheet("font : 8pt Courier");labelClass->setStyleSheet("font : 8pt Courier");comboxYear->setFixedSize(80,28);comboxClass->setFixedSize(100,28);conti->setFixedSize(90,30);conti->setFocusPolicy(Qt::NoFocus);
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
    comboxClass->setStyleSheet("QComboBox{subcontrol-origin: padding;subcontrol-position: top right;selection-background-color :rgb(255, 170, 0);background-color:white;selection-color: black;color: black;border-style: solid;border: 2px solid rgb(88, 88, 88);border-radius: 6;padding: 1px 0px 1px 20px;}"
    "QComboBox:hover{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:focus{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:on{border-radius : 5px;padding-top: 0px;padding-left: 0px;selection-background-color: rgb(255, 170, 0);color: black;selection-color :black;border : 1px solid rgb(84, 84, 84);}"
    "QComboBox:!on{color: black;}"
    "QComboBox QAbstractItemView{border: 2px solid darkgray;background-color:white;selection-color: black;selection-background-color: rgb(255, 170, 0);outline : 0;}"
    "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;width: 15px;color: white;border-left-width: 0px;border-left-color: darkgray;border-left-style: solid; border-top-right-radius: 6px;border-bottom-right-radius: 6px;padding-left: 10px;}"
    "QComboBox::down-arrow{width: 7px;height: 5px;}"
    "QComboBox QAbstractItemView::item:selected{border : 1px groove rgb(108, 108, 108);background-color: rgb(170, 170, 127);color : black;selection-color:black;}"
    "QComboBox QAbstractItemView::item{border : 1px groove rgb(108, 108, 108);color : black;selection-color:black;}");
    hLayout->addWidget(labelClass);hLayout->addWidget(comboxClass);hLayout->addWidget(labelYear);hLayout->addWidget(comboxYear);
    hLayout->addWidget(conti);
    hLayout->setGeometry(QRect(10,10,250,100));
    widget->setWindowModality(Qt::WindowModal);
    widget->setFixedSize(500,110);
    widget->setLayout(hLayout);
    if(this->inPrint == 1)
    {
        ui->tabWidget->insertTab(1,ui->tab_quanlysinhvien,"Quản Lý Sinh Viên");
        ui->tabWidget->setCurrentWidget(ui->tab_quanlysinhvien);

        comboxYear->clear();comboxClass->clear();
        labelClass->setText("Lớp :");
        qry.prepare("SELECT DISTINCT SCHOOL_YEAR FROM MARK_OF_STUDENTS WHERE MAMONHOC='"+this->professional+"'");
        query.prepare("SELECT DISTINCT CLASSNAME FROM MARK_OF_STUDENTS WHERE MAMONHOC='"+this->professional+"'");
        qry.exec();query.exec();
        while (qry.next()) {
            comboxYear->addItem(qry.value(0).toString());
        }
        while (query.next()) {
            comboxClass->addItem(query.value(0).toString());
        }
        widget->show();
    }
    if(this->inPrint == -1)
    {
        ui->tabWidget->insertTab(2,ui->tab_resStudent,"Danh sách sinh viên");
        ui->tabWidget->setCurrentWidget(ui->tab_resStudent);

        comboxYear->clear();comboxClass->clear();
        labelClass->setText("Mã HP:");
        query.prepare("SELECT DISTINCT semester FROM registered_course WHERE registered_course.subjectID ='"+this->professional+"'");
        qry.prepare("SELECT DISTINCT courseCode FROM registered_course WHERE registered_course.subjectID ='"+this->professional+"'");
        query.exec();qry.exec();
        while(query.next()){
                comboxYear->addItem(query.value(0).toString());
        }
        while (qry.next()) {
                comboxClass->addItem(qry.value(0).toString());
        }
        widget->show();
    }
}


void Teacher_Interactive_Interface::on_pushButton_8_clicked()
{
    setIndexOfTab(2);
    ui->tabWidget->setCurrentWidget(ui->tab_resStudent);
}


void Teacher_Interactive_Interface::on_pushButton_15_clicked()
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
    xlxs.write("E3","THỜI KHÓA BIỂU",format);
    xlxs.setRowHeight(3,30);
    xlxs.setRowHeight(6,20);
    formatCell.setPatternBackgroundColor(QColor(182, 182, 136));
    xlxs.mergeCells(CellRange("B9:B12"),formatCell);xlxs.mergeCells(CellRange("B14:B17"),formatCell);xlxs.mergeCells(CellRange("B19:B22"),formatCell);
    xlxs.write("B9","Sáng",formatCell);
    xlxs.write("B14","Trưa",formatCell);
    xlxs.write("B19","Tối",formatCell);
    formatCell.setBorderStyle(Format::BorderNone);
    xlxs.mergeCells("B4:C4",formatCell);
    xlxs.write("B4"," Mã học phần : S, Phòng học : P ",formatCell);

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

    if(saveCheck){messageBoxOneButton("Thông báo !","Xuất lịch dạy thành công !","Đồng ý",250,150,80,28);}
    else{
        messageBoxOneButton("Thông báo !","Xuất lịch dạy thất bại !","Đồng ý",250,150,80,28);}
}





void Teacher_Interactive_Interface::on_calendarWidget_clicked(const QDate &date)
{
    if(ui->lineEdit_dateStart->hasFocus()){
        qDebug()<<"START";
        ui->lineEdit_dateStart->setText(date.toString("dd-MM-yyyy"));
    }
    if(ui->lineEdit_dateEnd->hasFocus()){
        qDebug()<<"END";
        ui->lineEdit_dateEnd->setText(date.toString("dd-MM-yyyy"));
    }
    if(ui->lineEdit_dateTest->hasFocus()){
        qDebug()<<"TEST";
        ui->lineEdit_dateTest->setText(date.toString("dd-MM-yyyy"));
    }
    if(ui->lineEdit_dateStu->hasFocus()){
        qDebug()<<"Date Stud";
        ui->lineEdit_dateStu->setText(date.toString("dd-MM-yyyy"));
    }
}


void Teacher_Interactive_Interface::on_lineEdit_courseId_textChanged(const QString &arg1)
{
    if(checkCourseIdIsValid()){
        QToolTip::hideText();
        ui->lineEdit_courseId->setStyleSheet("color : black");
    }
    else{
        QToolTip::hideText();
        ui->lineEdit_courseId->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_courseId->mapToGlobal(QPoint()),tr("Mã học phần không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
    if(checkCourseIdIsExist()){
        QToolTip::hideText();
        ui->lineEdit_courseId->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_courseId->mapToGlobal(QPoint()),tr("Mã học phần đã tồn tại !"));
        QToolTip::setPalette(pal);
    }
    else{QToolTip::hideText();ui->lineEdit_courseId->setStyleSheet("color : black");}
}


void Teacher_Interactive_Interface::on_lineEdit_dateStart_textChanged(const QString &arg1)
{
    if (!checkDateStartIsValid())
    {
        QToolTip::hideText();
        ui->lineEdit_dateStart->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_dateStart->mapToGlobal(QPoint()),tr("Ngày bắt đầu không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
    else
    {
        QToolTip::hideText();
        ui->lineEdit_dateStart->setStyleSheet("color : black");
    }
}


void Teacher_Interactive_Interface::on_lineEdit_dateEnd_textChanged(const QString &arg1)
{
    string dater= ui->lineEdit_dateEnd->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_dateEnd->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_dateEnd->text(),"dd-MM-yyyy");
    if (!checkDateEndIsValid())
    {
        QToolTip::hideText();
        ui->lineEdit_dateEnd->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_dateEnd->mapToGlobal(QPoint()),tr("Ngày kết thúc không hợp lệ !"));
        QToolTip::setPalette(pal);
        //this->checkDatebirth = false;
    }
    else
    {
        QToolTip::hideText();
        ui->lineEdit_dateEnd->setStyleSheet("color : black");
    }
}


void Teacher_Interactive_Interface::on_lineEdit_dateTest_textChanged(const QString &arg1)
{
    string dater= ui->lineEdit_dateTest->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_dateTest->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_dateTest->text(),"dd-MM-yyyy");
    if (!checkDateTestIsValid())
    {
        QToolTip::hideText();
        ui->lineEdit_dateTest->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_dateTest->mapToGlobal(QPoint()),tr("Ngày thi không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
    else
    {
        QToolTip::hideText();
        ui->lineEdit_dateTest->setStyleSheet("color : black");
    }
}

void Teacher_Interactive_Interface::on_lineEdit_semester_textChanged(const QString &arg1)
{
    if(checkSemesterIsValid()){
        QToolTip::hideText();
        ui->lineEdit_semester->setStyleSheet("color : black");
    }
    else{
        QToolTip::hideText();
        ui->lineEdit_semester->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_semester->mapToGlobal(QPoint()),tr("Học kì không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
}

void Teacher_Interactive_Interface::on_lineEdit_quantity_textChanged(const QString &arg1)
{
    if(checkLimitedQuantityIsValid()){
        QToolTip::hideText();
        ui->lineEdit_quantity->setStyleSheet("color : black");
    }
    else{
        QToolTip::hideText();
        ui->lineEdit_quantity->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_quantity->mapToGlobal(QPoint()),tr("Số lượng không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
}


void Teacher_Interactive_Interface::on_pushButton_createCourse_clicked()
{
    qDebug()<<checkCourseIdIsExist()<<" "<<checkCourseIdIsValid()<<checkDateStartIsValid()<<checkDateEndIsValid()<<checkDateTestIsValid()<<checkSemesterIsValid()<<checkLimitedQuantityIsValid()<<checkAllDateIsValid()<<checkNumresIsValid();
    if(!checkCourseIdIsExist() && checkCourseIdIsValid() && checkDateStartIsValid() && checkDateEndIsValid() && checkDateTestIsValid() && checkSemesterIsValid() && checkLimitedQuantityIsValid() && checkAllDateIsValid() && checkNumresIsValid()){
            insertCourseIntoDatabase();
            if(ui->comboBox_ye->findText(ui->lineEdit_semester->text()) == -1){ui->comboBox_ye->insertItem(0,ui->lineEdit_semester->text());ui->comboBox_ye->model()->sort(0,Qt::DescendingOrder);};
            ui->comboBox_courseId->insertItem(0,ui->lineEdit_courseId->text());
            ui->comboBox_courseId->model()->sort(0,Qt::DescendingOrder);
            messageBoxOneButton("Thông báo !","Tạo học phần thành công !","Đồng ý",250,150,80,28);
            tableListOffer->select();
    }
    else{
        messageBoxOneButton("Thông báo !","Tạo học phần thất bại !","Đồng ý",250,150,80,28);
    }
}


void Teacher_Interactive_Interface::on_lineEdit_dateStu_textChanged(const QString &arg1)
{
    if(checkDateStudyIsValid()){
        QToolTip::hideText();
        ui->lineEdit_dateStu->setStyleSheet("color : black");
    }
    else{
        QToolTip::hideText();
        ui->lineEdit_dateStu->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_dateStu->mapToGlobal(QPoint()),tr("Ngày học không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
}


void Teacher_Interactive_Interface::on_lineEdit_class_textChanged(const QString &arg1)
{

    if(checkRoomIsValid()){
        QToolTip::hideText();
        ui->lineEdit_class->setStyleSheet("color : black");
    }
    else{
        QToolTip::hideText();
        ui->lineEdit_class->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_class->mapToGlobal(QPoint()),tr("Lớp học không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
}


void Teacher_Interactive_Interface::on_pushButton_createDay_clicked()
{
    if(checkDateStudyIsValid() && checkRoomIsValid() && checkDateStuValid() && checkSemesterIsChecked()){
            QString a = checkDateStudyIsExist();
            qDebug()<<"AAAAAAAAAAAA : "<<a;
            if(a == "all"){messageBoxOneButton("Thông báo !","Ca học này đã tồn tại !","Đồng ý",250,150,80,28);}
            if(a == "al"){messageBoxOneButton("Thông báo !","Ca học trùng với giảng viên khác !","Đồng ý",250,150,80,28);}
            if(a == "a"){
                insertScheduleOfferDatabase();
                messageBoxOneButton("Thông báo !","Tạo ngày học thành công !","Đồng ý",250,150,80,28);
                tableScheduleOffer->select();
            }
    }
    else{
        messageBoxOneButton("Thông báo !","Tạo ngày học phần thất bại !","Đồng ý",250,150,80,28);
    }
}


void Teacher_Interactive_Interface::on_tableView_listCourse_doubleClicked(const QModelIndex &index)
{
    QSqlQuery qury;QString id,sem;
    id = ui->tableView_listCourse->model()->index(index.row(),0,QModelIndex()).data().toString();
    sem = ui->tableView_listCourse->model()->index(index.row(),4,QModelIndex()).data().toString();
    qDebug()<<"OOOOOO->"<<id<<" "<<sem;
    qury.prepare("DELETE FROM scheduleTableOffer WHERE courseId = '"+id+"' AND semester = '"+sem+"'");
    bool it = messageBoxTwoButton("Thông báo !","Bạn chắc muốn xóa học phần này !","Đồng ý","Hủy",250,150,80,28);
    if(it){
        qury.exec();
        messageBoxOneButton("Thông báo !","Xóa học phần thành công !","Thoát",250,150,80,28);
        if(ui->tableView_listCourse->model()->removeRow(index.row(),QModelIndex())){

            tableListOffer->select();
            tableScheduleOffer->select();
        }
        else{
            messageBoxOneButton("Thông báo !","Xóa học phần không thành công !","Thoát",250,150,80,28);
        }
    }
}


void Teacher_Interactive_Interface::on_tableView_ScheduleCourse_doubleClicked(const QModelIndex &index)
{
    bool it = messageBoxTwoButton("Thông báo !","Bạn chắc muốn xóa ngày học này !","Đồng ý","Hủy",250,150,80,28);
    if(it){
        messageBoxOneButton("Thông báo !","Xóa ngày học thành công !","Thoát",250,150,80,28);
        if(ui->tableView_ScheduleCourse->model()->removeRow(index.row(),QModelIndex())){
            tableScheduleOffer->select();
        }
        else{
            messageBoxOneButton("Thông báo !","Xóa ngày học không thành công !","Thoát",250,150,80,28);
        }
    }
}


void Teacher_Interactive_Interface::on_action_xu_t_h_c_ph_n_triggered()
{
    ui->tabWidget->insertTab(4,ui->tab_courseProposal,"Đề xuất học phần");
    ui->tabWidget->setCurrentWidget(ui->tab_courseProposal);
}


void Teacher_Interactive_Interface::on_actionKh_i_ng_l_i_triggered()
{
    bool check = messageBoxTwoButton("Thông báo !","Bạn có chắc khởi động lại chương trình !","Đồng ý","Hủy",250,150,80,28);
    if(check){
        checkClose = false;
        this->close();
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}


void Teacher_Interactive_Interface::on_lineEdit_numRes_textChanged(const QString &arg1)
{
    if(checkNumresIsValid()){
        QToolTip::hideText();
        ui->lineEdit_numRes->setStyleSheet("color : black");
    }
    else{
        QToolTip::hideText();
        ui->lineEdit_numRes->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_numRes->mapToGlobal(QPoint()),tr("Số tín chỉ không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
}

