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
#include<string>
#include<string.h>
#include<vector>


//--import INSERT INTO ttefs SELECT * FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0','Excel 12.0; Database=C:\Users\duykh\Desktop\Book2.xlsx','SELECT * FROM [Sheet1$A1:D5]');
//--export INSERT INTO OPENROWSET('Microsoft.ACE.OLEDB.12.0','Excel 12.0; Database=C:\Users\duykh\Desktop\Book2.xlsx','SELECT * FROM [Sheet1$A1:D5]') SELECT * FROM ttefs;

Student_Interactive_Interface::Student_Interactive_Interface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Student_Interactive_Interface)
{
    ui->setupUi(this);
    wid = new QWidget();
    tableModelDebt = new QSqlTableModel();
    tableVieww = new QTableView();
    tbViewStudent = new QTableView();
    tbViewTeacher = new QTableView();
    tbViewDebtOfStudent = new QTableView();
    tbViewMarkStudent = new QTableView();tbViewRes = new QTableView();
    timer = new QTimer();
    this->indexForTableResA = -1;
    this->indexForTableResB = -1;
    qrymodall = new QSqlQueryModel();
    tableModel = new QSqlTableModel();
    tableModelRes = new QSqlTableModel();
    tableModelCanCRes = new QSqlTableModel();
    detailSchedule = new QSqlTableModel();tbManageMarkStudent = new QSqlTableModel();tbRes = new QSqlTableModel();

    tbCoursePassed = new QSqlTableModel();tbDebt = new QSqlTableModel();tbDebtOfStudent = new QSqlTableModel();
    tbTimeRes = new QSqlTableModel();
    updateInformation = new ChangeInformationOfStudent();
    tb = new QSqlTableModel();tbStudent = new QSqlTableModel();tbTeacher = new QSqlTableModel();
    hhLayout = new QHBoxLayout();hhLayout1 = new QHBoxLayout();vvLayout = new QVBoxLayout();vvLayout1 = new QVBoxLayout();vvLayout2 = new QVBoxLayout();hhhhhLayout = new QHBoxLayout();
    vvvLayout = new QVBoxLayout();hhhLayout = new QHBoxLayout();hhhhLayout = new QHBoxLayout();vvvvvLayout = new QVBoxLayout();
    vvvvLayout = new QVBoxLayout();
    labelId = new QLabel("Mã ->");labelName = new QLabel("Họ tên ->");labelMajor = new QLabel("Chuyên ngành ->");labelCLass = new QLabel("Lớp ->");
    labelClas = new QLabel("Lớp :");labelSemester = new QLabel("Học kì :");lbSub = new QLabel("Môn học :");lbSemester = new QLabel("Năm học :");
    createPerson = new QPushButton("Tạo mới");viewButton = new QPushButton("Xem");viewRes = new QPushButton("Xem");
    lineEditId = new QLineEdit();lineEditName = new QLineEdit();lineEditMajor = new QLineEdit();lineEditClass = new QLineEdit();
    comboxYear = new QComboBox();comboxSemester = new QComboBox();comboxClass = new QComboBox();comSemester = new QComboBox();comSub = new QComboBox();
    labelYear = new QLabel("Năm học :");
    widget = new QWidget();widMarkStudent = new QWidget();widRes = new QWidget();
    widAddNewTeacher = new QWidget();widDebtOfStudent = new QWidget();checkTime = false;
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(UpdateTime()));
    timer->start(1000);
    tbViewMarkStudent->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->tableView_student,&QTableView::customContextMenuRequested,this,&Student_Interactive_Interface::showUpConTextMenu1);
    QObject::connect(ui->tableView_teacher,&QTableView::customContextMenuRequested,this,&Student_Interactive_Interface::showUpConTextMenu);
    QObject::connect(ui->tableView_registerCourse,&QTableView::customContextMenuRequested,this,&Student_Interactive_Interface::showUpConTextMenu2);
    QObject::connect(tbViewMarkStudent,&QTableView::customContextMenuRequested,this,&Student_Interactive_Interface::showUpConTextMenu3);
    QObject::connect(tbViewDebtOfStudent,&QTableView::customContextMenuRequested,this,&Student_Interactive_Interface::showUpConTextMenu4);
    QObject::connect(tbViewRes,&QTableView::customContextMenuRequested,this,&Student_Interactive_Interface::showUpConTextMenu6);
    QObject::connect(createPerson,&QPushButton::clicked,this,&Student_Interactive_Interface::checkNewTeacherIsValid);
    QObject::connect(lineEditId,&QLineEdit::textChanged,this,&Student_Interactive_Interface::checkIDIsExisted);
    QObject::connect(updateInformation,&QWidget::destroyed,this,&Student_Interactive_Interface::freshTable);
    QObject::connect(ui->tabWidget,&QTabWidget::currentChanged,this,&Student_Interactive_Interface::ifComboBoxNull);
    QObject::connect(wid,&QWidget::destroyed,this,&Student_Interactive_Interface::clearSelection);
    QObject::connect(ui->comboBox_2,&QComboBox::currentTextChanged,this,&Student_Interactive_Interface::timeResChange);
    QObject::connect(lineEditMajor,&QLineEdit::textChanged,this,&Student_Interactive_Interface::textLineEditMajorChange);
    QObject::connect(lineEditId,&QLineEdit::textChanged,this,&Student_Interactive_Interface::textLineEditIDChange);
    QObject::connect(comboxYear,&QComboBox::currentTextChanged,this,&Student_Interactive_Interface::comboboxDebtChange);
    QObject::connect(tbViewDebtOfStudent,&QTableView::doubleClicked,this,&Student_Interactive_Interface::tableDebtDoubleClick);
    QObject::connect(viewButton,&QPushButton::clicked,this,&Student_Interactive_Interface::viewButtonClicked);
    QObject::connect(tbViewStudent,&QTableView::doubleClicked,this,&Student_Interactive_Interface::doubleClickedOnTbStudent);
    QObject::connect(tbViewTeacher,&QTableView::doubleClicked,this,&Student_Interactive_Interface::doubleClickedOnTbTeacher);
    QObject::connect(viewRes,&QPushButton::clicked,this,&Student_Interactive_Interface::viewResClicked);
    QObject::connect(ui->comboBox,&QComboBox::currentTextChanged,this,&Student_Interactive_Interface::refreshListCourse);
    QObject::connect(ui->lineEdit_id,&QLineEdit::textChanged,this,&Student_Interactive_Interface::idTrue);
    ui->tableView_registerCourse->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_registeredCourse->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableVieww->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableVieww->setWordWrap(true);
    QLabel* label = new QLabel("");
    label->setFixedWidth(33);
    label->setFixedHeight(180);
    ui->toolBar->insertWidget(ui->actionTh_ng_tin_sinh_vi_n,label);
    tbA = -1;tbB = -1;nj = false,mj=false;
    Init();
    ui->verticalSpacerL->changeSize(40,560);
    ui->verticalSpacerP->changeSize(40,100);
}

Student_Interactive_Interface::~Student_Interactive_Interface()
{
    delete ui;
    delete timer;
    delete qrymodall;
    delete tableModel;
    delete tableModelRes;
    delete wid;delete widget;
    delete tableModelDebt;delete detailSchedule;
    delete tableVieww;delete tbViewDebtOfStudent;
    delete updateInformation;
    delete chartView;
    delete axis;delete chart;delete series;delete axisY;delete axisX;
    delete set0;delete set1;delete set2;delete set3;delete set4;delete set5;delete set6;delete set7;delete set8;delete set9;delete set10;
    delete comboxYear,delete labelYear,delete conti;delete labelCLass;delete lineEditClass;
    delete vLayout;delete hLayout;delete hhLayout;delete vvLayout;delete vvLayout1;delete vvLayout2;delete hhLayout1;delete vvvLayout;delete hhhLayout;
    delete tb;delete tableModelCanCRes;delete widAddNewTeacher;
    delete labelId;delete labelName;delete labelMajor;
    delete lineEditId;delete lineEditName;delete lineEditMajor;delete createPerson;
    delete tbStudent;delete tbTeacher;delete tbViewStudent;delete tbViewTeacher;
    delete tbCoursePassed,tbDebt;delete tbTimeRes;delete tbDebtOfStudent;delete tbManageMarkStudent;
    delete labelSemester,labelClas,comboxClass,comboxSemester,viewButton,hhhhLayout,vvvvLayout;
    delete widMarkStudent;delete tbViewMarkStudent;
    delete tbRes,tbViewRes,hhhhhLayout,vvvvvLayout,comSub,comSemester,lbSub,lbSemester,viewRes;
    mapp.clear();
}




//// init
void Student_Interactive_Interface::Init()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->comboBox_yearSch->hide();
    ui->label_10->hide();
    tableModelDebt->setTable("scheduleTableOffer");
    initComboBox();
    initComboBoxForMC();
    initOptionsComboBox();
    initTableManageStudent();
    initTableManageTeacher();
    initTableManageCourse();
    initTableManageCoursePassed();
    initTableTimeRes();
    initWidAddNewPerson();
    initTableStudent();
    initTableTeacher();
    initTableCoursePassed();
    initTableDebt();
    initTableDebtOfStudent();
    initSubRes();
    initComboBoxDebt();
    initComboBoxCourse();
    initComboBoxTimeRes();
    initComboBoxDebtOfStudent();
    initAllTableMark();
    initComboBoxMark();
    initComboRes();
    detailCourse();
}

void Student_Interactive_Interface::initTableManageStudent()
{
    tb->setQuery("SELECT * FROM Student_DATABASE ORDER BY MSSV ASC");
    ui->tableView_student->setModel(tb);
    ui->tabWidget->setCurrentIndex(1);
    ui->tableView_student->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_teacher->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_registerCourse->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_registerCourse->verticalHeader()->hide();
    ui->tableView_registeredCourse->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_registeredCourse->verticalHeader()->hide();
    ui->tableView_openCourse->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_openCourse->verticalHeader()->hide();
    ui->tableView_listDebt->horizontalHeader()->setSectionsClickable(false);
    ui->tableView_listDebt->verticalHeader()->hide();
    ui->tableView_teacher->verticalHeader()->hide();
    ui->tableView_student->verticalHeader()->hide();
    ui->tableView_student->model()->setHeaderData(0,Qt::Horizontal,tr("Mã sinh viên"));
    ui->tableView_student->model()->setHeaderData(1,Qt::Horizontal,tr("Họ tên"));
    ui->tableView_student->model()->setHeaderData(2,Qt::Horizontal,tr("Lớp"));
    ui->tableView_student->model()->setHeaderData(3,Qt::Horizontal,tr("Chuyên ngành"));
    ui->tableView_student->model()->setHeaderData(4,Qt::Horizontal,tr("Email"));
    ui->tableView_student->model()->setHeaderData(5,Qt::Horizontal,tr("Giới tính"));
    ui->tableView_student->model()->setHeaderData(6,Qt::Horizontal,tr("Số điện thoại"));
    ui->tableView_student->model()->setHeaderData(7,Qt::Horizontal,tr("Địa chỉ"));
    ui->tableView_student->model()->setHeaderData(10,Qt::Horizontal,tr("Căn cước công dân"));
    ui->tableView_student->model()->setHeaderData(11,Qt::Horizontal,tr("Ngày sinh"));
    ui->tableView_student->model()->setHeaderData(12,Qt::Horizontal,tr("Dân tộc"));
    ui->tableView_student->model()->setHeaderData(13,Qt::Horizontal,tr("Nơi sinh"));
    ui->tableView_student->hideColumn(8);
    ui->tableView_student->hideColumn(9);
    ui->tableView_student->setColumnWidth(0,150);
    ui->tableView_student->setColumnWidth(1,200);
    ui->tableView_student->setColumnWidth(2,100);
    ui->tableView_student->setColumnWidth(3,350);
    ui->tableView_student->setColumnWidth(4,200);
    ui->tableView_student->setColumnWidth(5,100);
    ui->tableView_student->setColumnWidth(6,150);
    ui->tableView_student->setColumnWidth(7,300);
    ui->tableView_student->setColumnWidth(10,150);
    ui->tableView_student->setColumnWidth(11,150);
    ui->tableView_student->setColumnWidth(12,150);
    ui->tableView_student->setColumnWidth(13,300);


}

void Student_Interactive_Interface::initTableManageTeacher()
{
    tableModel->setQuery("SELECT * FROM Lecturer_DATABASE ORDER BY MSGV ASC");
    ui->tableView_teacher->setModel(tableModel);
    ui->tabWidget->setCurrentIndex(0);
    ui->tableView_teacher->model()->setHeaderData(0,Qt::Horizontal,tr("Mã giáo viên"));
    ui->tableView_teacher->model()->setHeaderData(1,Qt::Horizontal,tr("Họ tên"));
    ui->tableView_teacher->model()->setHeaderData(2,Qt::Horizontal,tr("Chuyên ngành"));
    ui->tableView_teacher->model()->setHeaderData(3,Qt::Horizontal,tr("Căn cước công dân"));
    ui->tableView_teacher->model()->setHeaderData(4,Qt::Horizontal,tr("Số điện thoại"));
    ui->tableView_teacher->model()->setHeaderData(5,Qt::Horizontal,tr("Email"));
    ui->tableView_teacher->model()->setHeaderData(6,Qt::Horizontal,tr("Địa chỉ"));
    ui->tableView_teacher->model()->setHeaderData(7,Qt::Horizontal,tr("Giới tính"));
    ui->tableView_teacher->model()->setHeaderData(8,Qt::Horizontal,tr("Ngày sinh"));
    ui->tableView_teacher->model()->setHeaderData(11,Qt::Horizontal,tr("Dân tộc"));
    ui->tableView_teacher->model()->setHeaderData(12,Qt::Horizontal,tr("Nơi sinh"));
    ui->tableView_teacher->hideColumn(9);
    ui->tableView_teacher->hideColumn(10);
    ui->tableView_teacher->setColumnWidth(0,150);
    ui->tableView_teacher->setColumnWidth(1,200);
    ui->tableView_teacher->setColumnWidth(2,350);
    ui->tableView_teacher->setColumnWidth(3,150);
    ui->tableView_teacher->setColumnWidth(4,150);
    ui->tableView_teacher->setColumnWidth(5,200);
    ui->tableView_teacher->setColumnWidth(6,300);
    ui->tableView_teacher->setColumnWidth(7,100);
    ui->tableView_teacher->setColumnWidth(8,150);
    ui->tableView_teacher->setColumnWidth(11,100);
    ui->tableView_teacher->setColumnWidth(12,300);
}

void Student_Interactive_Interface::initTableManageCourse()
{
    tableModelRes->setTable("listCourseOffer");
    tableModelRes->setSort(0,Qt::AscendingOrder);
    tableModelRes->setFilter("semester = '"+ui->comboBox->currentText()+"'");
    tableModelRes->select();
    ui->tableView_registerCourse->setModel(tableModelRes);
    ui->tableView_registerCourse->hideColumn(4);
    ui->tableView_registerCourse->model()->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    ui->tableView_registerCourse->model()->setHeaderData(1,Qt::Horizontal,tr("Ngày bắt đầu"));
    ui->tableView_registerCourse->model()->setHeaderData(2,Qt::Horizontal,tr("Ngày kết thúc"));
    ui->tableView_registerCourse->model()->setHeaderData(3,Qt::Horizontal,tr("Ngày thi"));
    ui->tableView_registerCourse->model()->setHeaderData(5,Qt::Horizontal,tr("Số lượng tối đa"));
    ui->tableView_registerCourse->model()->setHeaderData(6,Qt::Horizontal,tr("Mã môn học"));
    ui->tableView_registerCourse->model()->setHeaderData(7,Qt::Horizontal,tr("Số tín chỉ"));
    ui->tableView_registerCourse->model()->setHeaderData(8,Qt::Horizontal,tr("Mã giảng viên"));
    ui->tableView_registerCourse->setColumnWidth(0,100);
    ui->tableView_registerCourse->setColumnWidth(1,150);
    ui->tableView_registerCourse->setColumnWidth(2,150);
    ui->tableView_registerCourse->setColumnWidth(3,150);
    ui->tableView_registerCourse->setColumnWidth(4,100);
    ui->tableView_registerCourse->setColumnWidth(5,130);
    ui->tableView_registerCourse->setColumnWidth(6,100);
    ui->tableView_registerCourse->setColumnWidth(7,100);
    ui->tableView_registerCourse->setColumnWidth(8,130);
}

void Student_Interactive_Interface::initTableManageCoursePassed()
{
    tableModelCanCRes->setTable("CourseHasPassed");
    tableModelCanCRes->setSort(0,Qt::AscendingOrder);
    tableModelCanCRes->setFilter("semester = '"+ui->comboBox->currentText()+"'");
    tableModelCanCRes->select();
    ui->tableView_registeredCourse->setModel(tableModelCanCRes);
    ui->tableView_registeredCourse->hideColumn(4);
    ui->tableView_registeredCourse->model()->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    ui->tableView_registeredCourse->model()->setHeaderData(1,Qt::Horizontal,tr("Ngày bắt đầu"));
    ui->tableView_registeredCourse->model()->setHeaderData(2,Qt::Horizontal,tr("Ngày kết thúc"));
    ui->tableView_registeredCourse->model()->setHeaderData(3,Qt::Horizontal,tr("Ngày thi"));
    ui->tableView_registeredCourse->model()->setHeaderData(5,Qt::Horizontal,tr("Số lượng tối đa"));
    ui->tableView_registeredCourse->model()->setHeaderData(6,Qt::Horizontal,tr("Mã môn học"));
    ui->tableView_registeredCourse->model()->setHeaderData(7,Qt::Horizontal,tr("Số tín chỉ"));
    ui->tableView_registeredCourse->model()->setHeaderData(8,Qt::Horizontal,tr("Mã giảng viên"));
    ui->tableView_registeredCourse->model()->setHeaderData(9,Qt::Horizontal,tr("Trạng thái"));
    ui->tableView_registeredCourse->setColumnWidth(0,100);
    ui->tableView_registeredCourse->setColumnWidth(1,150);
    ui->tableView_registeredCourse->setColumnWidth(2,150);
    ui->tableView_registeredCourse->setColumnWidth(3,150);
    ui->tableView_registeredCourse->setColumnWidth(4,100);
    ui->tableView_registeredCourse->setColumnWidth(5,130);
    ui->tableView_registeredCourse->setColumnWidth(6,100);
    ui->tableView_registeredCourse->setColumnWidth(7,100);
    ui->tableView_registeredCourse->setColumnWidth(8,130);
    ui->tableView_registeredCourse->setColumnWidth(9,100);
}

void Student_Interactive_Interface::initTableCoursePassed()
{
    tbCoursePassed->setTable("registerForTheCourse");
    tbCoursePassed->setSort(4,Qt::DescendingOrder);
    tbCoursePassed->select();
    ui->tableView_openCourse->setModel(tbCoursePassed);
    ui->tableView_openCourse->model()->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    ui->tableView_openCourse->model()->setHeaderData(1,Qt::Horizontal,tr("Tên môn học"));
    ui->tableView_openCourse->model()->setHeaderData(2,Qt::Horizontal,tr("Ngày bắt đầu"));
    ui->tableView_openCourse->model()->setHeaderData(3,Qt::Horizontal,tr("Ngày kết thúc"));
    ui->tableView_openCourse->model()->setHeaderData(4,Qt::Horizontal,tr("Học kì"));
    ui->tableView_openCourse->model()->setHeaderData(5,Qt::Horizontal,tr("Số lượng tối đa"));
    ui->tableView_openCourse->model()->setHeaderData(6,Qt::Horizontal,tr("Đã đăng kí"));
    ui->tableView_openCourse->model()->setHeaderData(7,Qt::Horizontal,tr("Tên giảng viên"));
    ui->tableView_openCourse->model()->setHeaderData(8,Qt::Horizontal,tr("Mã giáo viên"));
    ui->tableView_openCourse->model()->setHeaderData(9,Qt::Horizontal,tr("Số tín chỉ"));
    ui->tableView_openCourse->setColumnWidth(0,100);
    ui->tableView_openCourse->setColumnWidth(1,250);
    ui->tableView_openCourse->setColumnWidth(2,150);
    ui->tableView_openCourse->setColumnWidth(3,150);
    ui->tableView_openCourse->setColumnWidth(4,100);
    ui->tableView_openCourse->setColumnWidth(5,130);
    ui->tableView_openCourse->setColumnWidth(6,100);
    ui->tableView_openCourse->setColumnWidth(7,200);
    ui->tableView_openCourse->setColumnWidth(8,130);
    ui->tableView_openCourse->setColumnWidth(8,130);
}

void Student_Interactive_Interface::initTableDebt()
{
    tbDebt->setTable("DEBT");
    tbDebt->setSort(3,Qt::DescendingOrder);
    tbDebt->select();
    ui->tableView_listDebt->setModel(tbDebt);
    ui->tableView_listDebt->model()->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    ui->tableView_listDebt->model()->setHeaderData(1,Qt::Horizontal,tr("Mã môn học"));
    ui->tableView_listDebt->model()->setHeaderData(2,Qt::Horizontal,tr("Công nợ"));
    ui->tableView_listDebt->model()->setHeaderData(3,Qt::Horizontal,tr("Học kì"));
    ui->tableView_listDebt->setColumnWidth(0,105);
    ui->tableView_listDebt->setColumnWidth(1,120);
    ui->tableView_listDebt->setColumnWidth(2,120);
    ui->tableView_listDebt->setColumnWidth(3,105);
    ui->lineEdit_debt->setMaxLength(10);
    ui->lineEdit_semester->setMaxLength(5);
}

void Student_Interactive_Interface::initTableTimeRes()
{
    tbTimeRes->setTable("timeToRegis");
    tbTimeRes->setSort(0,Qt::AscendingOrder);
    tbTimeRes->select();
    ui->tableView_timeRes->setModel(tbTimeRes);
    ui->tableView_timeRes->verticalHeader()->hide();
    ui->tableView_timeRes->model()->setHeaderData(0,Qt::Horizontal,"Năm học");
    ui->tableView_timeRes->model()->setHeaderData(1,Qt::Horizontal,"Thời hạn đăng kí");
    ui->tableView_timeRes->setColumnWidth(0,127);
    ui->tableView_timeRes->setColumnWidth(1,200);
    ui->tableView_timeRes->horizontalHeader()->setSectionsClickable(false);
}

void Student_Interactive_Interface::initTableDebtOfStudent()
{
    initSubDebtTable();widDebtOfStudent->setWindowIcon(QIcon(":/icon-file/debt_student_standard.png"));
    labelYear->setFixedSize(80,30);comboxYear->setFixedSize(90,30);labelYear->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);comboxYear->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    hhLayout->setSizeConstraint(QLayout::SetFixedSize);
    hhhLayout->addWidget(labelYear);hhhLayout->addWidget(comboxYear);tbViewDebtOfStudent->setEditTriggers(QTableView::NoEditTriggers);
    hhhLayout->setSpacing(0);hhLayout->setAlignment(Qt::AlignHCenter);
    hhhLayout->setContentsMargins(200,0,200,0);
    vvvLayout->addItem(hhhLayout);vvvLayout->addWidget(tbViewDebtOfStudent);
    widDebtOfStudent->setLayout(vvvLayout);
    widDebtOfStudent->setFixedSize(650,450);
}

void Student_Interactive_Interface::initSubDebtTable()
{
    tbDebtOfStudent->setTable("debtOfStudent");
    tbDebtOfStudent->setSort(0,Qt::AscendingOrder);
    tbDebtOfStudent->select();
    tbViewDebtOfStudent->setModel(tbDebtOfStudent);
    tbViewDebtOfStudent->verticalHeader()->hide();
    tbViewDebtOfStudent->model()->setHeaderData(0,Qt::Horizontal,"Mã học phần");
    tbViewDebtOfStudent->model()->setHeaderData(1,Qt::Horizontal,"Mã môn học");
    tbViewDebtOfStudent->model()->setHeaderData(2,Qt::Horizontal,"Công nợ");
    tbViewDebtOfStudent->model()->setHeaderData(3,Qt::Horizontal,"Số tín chỉ");
    tbViewDebtOfStudent->model()->setHeaderData(4,Qt::Horizontal,"Tổng công nợ");
    tbViewDebtOfStudent->model()->setHeaderData(5,Qt::Horizontal,"Mã sinh viên");
    tbViewDebtOfStudent->model()->setHeaderData(6,Qt::Horizontal,"Học kì");
    tbViewDebtOfStudent->model()->setHeaderData(7,Qt::Horizontal,"Trạng thái");
    tbViewDebtOfStudent->setColumnWidth(0,100);
    tbViewDebtOfStudent->setColumnWidth(1,120);
    tbViewDebtOfStudent->setColumnWidth(2,120);
    tbViewDebtOfStudent->setColumnWidth(3,100);
    tbViewDebtOfStudent->setColumnWidth(4,120);
    tbViewDebtOfStudent->setColumnWidth(5,120);
    tbViewDebtOfStudent->setColumnWidth(6,100);
    tbViewDebtOfStudent->setColumnWidth(7,120);
    tbViewDebtOfStudent->setFocusPolicy(Qt::NoFocus);
    tbViewDebtOfStudent->horizontalHeader()->setSectionsClickable(false);
    tbViewDebtOfStudent->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbViewDebtOfStudent->setSelectionMode(QAbstractItemView::SingleSelection);
    widDebtOfStudent->setStyleSheet("QHeaderView{font:8pt Courier;border:none;border-bottom:1px solid}"
    "QHeaderView::section{font:8pt Courier;border:none;background-color: rgb(240, 255, 255);border-left-color:black;border-left : 1px solid;}"
    "QTableView{font:8pt Courier;background-color: #ecec76; selection-background-color: #d0d09c; selection-color: black;}"
    "QLabel{font : 8pt Courier;background-color: rgb(255, 170, 0);border:none}"
    "QComboBox{font : 8pt Courier;subcontrol-origin: padding;subcontrol-position: top right;selection-background-color :rgb(255, 170, 0);background-color:white;selection-color: black;color: black;border-style: solid;border: 2px solid rgb(88, 88, 88);border-radius: 6;padding: 1px 0px 1px 20px;}"
    "QComboBox:hover{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:focus{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:on{border-radius : 5px;padding-top: 0px;padding-left: 0px;selection-background-color: rgb(255, 170, 0);color: black;selection-color :black;}"
    "QComboBox:!on{color: black;}"
    "QComboBox QAbstractItemView{border: 2px solid darkgray;background-color:white;selection-color: black;selection-background-color: rgb(255, 170, 0);outline : 0;}"
    "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;width: 15px;color: white;border-left-width: 0px;border-left-color: darkgray;border-left-style: solid;border-top-right-radius: 6px;border-bottom-right-radius: 6px;padding-left: 10px;}"
    "QComboBox::down-arrow{width: 7px;height: 5px;}"
    "QComboBox QAbstractItemView::item:selected{border : 1px groove rgb(108, 108, 108);background-color: rgb(170, 170, 127);color : black;selection-color:black;}"
    "QComboBox QAbstractItemView::item{border : 1px groove rgb(108, 108, 108);color : black;selection-color:black;}");
    widDebtOfStudent->setWindowTitle("Công nợ sinh viên");tbViewDebtOfStudent->setContextMenuPolicy(Qt::CustomContextMenu);
    widDebtOfStudent->setWindowModality(Qt::ApplicationModal);
}

void Student_Interactive_Interface::initTableManageMarkStudent()
{
    tbManageMarkStudent->setTable("MARK_OF_STUDENTS");
    tbManageMarkStudent->setSort(0,Qt::AscendingOrder);
    tbManageMarkStudent->select();
    tbViewMarkStudent->setModel(tbManageMarkStudent);
    tbViewMarkStudent->verticalHeader()->hide();
    tbViewMarkStudent->horizontalHeader()->setSectionsClickable(false);
    tbViewMarkStudent->setFocusPolicy(Qt::NoFocus);
    tbViewMarkStudent->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbViewMarkStudent->setSelectionMode(QAbstractItemView::SingleSelection);
    tbViewMarkStudent->model()->setHeaderData(0,Qt::Horizontal,"Mã sinh viên");
    tbViewMarkStudent->model()->setHeaderData(1,Qt::Horizontal,"Mã môn học");
    tbViewMarkStudent->model()->setHeaderData(2,Qt::Horizontal,"Năm học");
    tbViewMarkStudent->model()->setHeaderData(3,Qt::Horizontal,"Lớp");
    tbViewMarkStudent->model()->setHeaderData(4,Qt::Horizontal,"Điểm lần 1");
    tbViewMarkStudent->model()->setHeaderData(5,Qt::Horizontal,"Điểm lần 2");
    tbViewMarkStudent->model()->setHeaderData(6,Qt::Horizontal,"Điểm lần 3");
    tbViewMarkStudent->model()->setHeaderData(7,Qt::Horizontal,"Thường xuyên");
    tbViewMarkStudent->model()->setHeaderData(8,Qt::Horizontal,"Cuối kì");
    tbViewMarkStudent->model()->setHeaderData(9,Qt::Horizontal,"Tổng kết");
    tbViewMarkStudent->model()->setHeaderData(10,Qt::Horizontal,"Điểm 4");
    tbViewMarkStudent->model()->setHeaderData(11,Qt::Horizontal,"Điểm chữ");
    tbViewMarkStudent->model()->setHeaderData(12,Qt::Horizontal,"Xếp loại");
    tbViewMarkStudent->setColumnWidth(0,120);tbViewMarkStudent->setColumnWidth(1,120);
    tbViewMarkStudent->setColumnWidth(2,100);tbViewMarkStudent->setColumnWidth(3,100);
    tbViewMarkStudent->setColumnWidth(4,120);tbViewMarkStudent->setColumnWidth(4,120);
    tbViewMarkStudent->setColumnWidth(6,120);tbViewMarkStudent->setColumnWidth(7,120);
    tbViewMarkStudent->setColumnWidth(8,120);tbViewMarkStudent->setColumnWidth(9,120);
    tbViewMarkStudent->setColumnWidth(10,120);tbViewMarkStudent->setColumnWidth(11,120);
    tbViewMarkStudent->setColumnWidth(12,120);
    tbViewMarkStudent->setEditTriggers(QTableView::NoEditTriggers);
    widMarkStudent->setStyleSheet("QHeaderView{font:8pt Courier;border:none;border-bottom:1px solid}"
    "QHeaderView::section{font:8pt Courier;border:none;background-color: rgb(240, 255, 255);border-left-color:black;border-left : 1px solid;}"
    "QTableView{font:8pt Courier;background-color: #ecec76; selection-background-color: #d0d09c; selection-color: black;}"
    "QLabel{font : 8pt Courier;background-color: rgb(255, 170, 0);border:none}"
    "QComboBox{font : 8pt Courier;subcontrol-origin: padding;subcontrol-position: top right;selection-background-color :rgb(255, 170, 0);background-color:white;selection-color: black;color: black;border-style: solid;border: 2px solid rgb(88, 88, 88);border-radius: 6;padding: 1px 0px 1px 20px;}"
    "QComboBox:hover{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:focus{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:on{border-radius : 5px;padding-top: 0px;padding-left: 0px;selection-background-color: rgb(255, 170, 0);color: black;selection-color :black;}"
    "QComboBox:!on{color: black;}"
    "QComboBox QAbstractItemView{border: 2px solid darkgray;background-color:white;selection-color: black;selection-background-color: rgb(255, 170, 0);outline : 0;}"
    "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;width: 15px;color: white;border-left-width: 0px;border-left-color: darkgray;border-left-style: solid;border-top-right-radius: 6px;border-bottom-right-radius: 6px;padding-left: 10px;}"
    "QComboBox::down-arrow{width: 7px;height: 5px;}"
    "QComboBox QAbstractItemView::item:selected{border : 1px groove rgb(108, 108, 108);background-color: rgb(170, 170, 127);color : black;selection-color:black;}"
    "QComboBox QAbstractItemView::item{border : 1px groove rgb(108, 108, 108);color : black;selection-color:black;}"
    "QPushButton{font : 8pt Courier;background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{font : 8pt Courier;background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{font : 8pt Courier;background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}"
    "QPushButton:pressed{border:1px solid rgb(162, 108, 0);outline : 0;}");widMarkStudent->setContextMenuPolicy(Qt::CustomContextMenu);
    tbViewMarkStudent->setContextMenuPolicy(Qt::CustomContextMenu);
}

void Student_Interactive_Interface::initAllTableMark()
{
    initTableManageMarkStudent();widMarkStudent->setWindowIcon(QIcon(":/icon-file/result_pic_standard.png"));
    widMarkStudent->setWindowModality(Qt::ApplicationModal);widMarkStudent->setFixedSize(800,500);widMarkStudent->setWindowTitle("Danh sách điểm sinh viên");
    comboxSemester->setFocusPolicy(Qt::NoFocus);comboxClass->setFocusPolicy(Qt::NoFocus);
    viewButton->setFocusPolicy(Qt::NoFocus);viewButton->setFixedSize(100,30);
    labelSemester->setFixedSize(80,30);labelClas->setFixedSize(80,30);
    comboxSemester->setFixedSize(100,30);comboxClass->setFixedSize(100,30);
    hhhhLayout->addWidget(labelSemester);hhhhLayout->addWidget(comboxSemester);
    hhhhLayout->setContentsMargins(150,0,150,0);
    hhhhLayout->addWidget(labelClas);hhhhLayout->addWidget(comboxClass);hhhhLayout->addWidget(viewButton);
    vvvvLayout->addItem(hhhhLayout);vvvvLayout->addWidget(tbViewMarkStudent);
    widMarkStudent->setLayout(vvvvLayout);

}

void Student_Interactive_Interface::initTableRes()
{
    tbRes->setTable("registered_course");
    tbRes->setSort(0,Qt::AscendingOrder);
    tbRes->select();
    tbViewRes->setModel(tbRes);
    tbViewRes->verticalHeader()->hide();tbViewRes->horizontalHeader()->setSectionsClickable(false);
    tbViewRes->setSelectionBehavior(QAbstractItemView::SelectRows);tbViewRes->setSelectionMode(QAbstractItemView::SingleSelection);tbViewRes->setFocusPolicy(Qt::NoFocus);
    tbViewRes->model()->setHeaderData(0,Qt::Horizontal,"Mã học phần");
    tbViewRes->model()->setHeaderData(1,Qt::Horizontal,"Mã môn học");
    tbViewRes->model()->setHeaderData(2,Qt::Horizontal,"Ngày bắt đầu");
    tbViewRes->model()->setHeaderData(3,Qt::Horizontal,"Ngày kết thúc");
    tbViewRes->model()->setHeaderData(4,Qt::Horizontal,"Học kì");
    tbViewRes->model()->setHeaderData(5,Qt::Horizontal,"Mã sinh viên");
    tbViewRes->model()->setHeaderData(6,Qt::Horizontal,"Giảng viên");
    tbViewRes->model()->setHeaderData(7,Qt::Horizontal,"Mã giảng viên");
    tbViewRes->model()->setHeaderData(8,Qt::Horizontal,"Số tin chỉ");
    tbViewRes->setColumnWidth(0,100);tbViewRes->setColumnWidth(1,150);
    tbViewRes->setColumnWidth(2,150);tbViewRes->setColumnWidth(3,150);
    tbViewRes->setColumnWidth(4,100);tbViewRes->setColumnWidth(5,150);
    tbViewRes->setColumnWidth(6,150);tbViewRes->setColumnWidth(7,150);tbViewRes->setColumnWidth(8,100);

    widRes->setStyleSheet("QHeaderView{font:8pt Courier;border:none;border-bottom:1px solid}"
    "QHeaderView::section{font:8pt Courier;border:none;background-color: rgb(240, 255, 255);border-left-color:black;border-left : 1px solid;}"
    "QTableView{font:8pt Courier;background-color: #ecec76; selection-background-color: #d0d09c; selection-color: black;}"
    "QLabel{font : 8pt Courier;background-color: rgb(255, 170, 0);border:none}"
    "QComboBox{font : 8pt Courier;subcontrol-origin: padding;subcontrol-position: top right;selection-background-color :rgb(255, 170, 0);background-color:white;selection-color: black;color: black;border-style: solid;border: 2px solid rgb(88, 88, 88);border-radius: 6;padding: 1px 0px 1px 20px;}"
    "QComboBox:hover{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:focus{border:2px solid rgb(255, 170, 0);color: black;outline : none;}"
    "QComboBox:on{border-radius : 5px;padding-top: 0px;padding-left: 0px;selection-background-color: rgb(255, 170, 0);color: black;selection-color :black;}"
    "QComboBox:!on{color: black;}"
    "QComboBox QAbstractItemView{border: 2px solid darkgray;background-color:white;selection-color: black;selection-background-color: rgb(255, 170, 0);outline : 0;}"
    "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;width: 15px;color: white;border-left-width: 0px;border-left-color: darkgray;border-left-style: solid;border-top-right-radius: 6px;border-bottom-right-radius: 6px;padding-left: 10px;}"
    "QComboBox::down-arrow{width: 7px;height: 5px;}"
    "QComboBox QAbstractItemView::item:selected{border : 1px groove rgb(108, 108, 108);background-color: rgb(170, 170, 127);color : black;selection-color:black;}"
    "QComboBox QAbstractItemView::item{border : 1px groove rgb(108, 108, 108);color : black;selection-color:black;}"
    "QPushButton{font : 8pt Courier;background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{font : 8pt Courier;background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{font : 8pt Courier;background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}"
    "QPushButton:pressed{border:1px solid rgb(162, 108, 0);outline : 0;}");
    tbViewRes->setContextMenuPolicy(Qt::CustomContextMenu);
}

void Student_Interactive_Interface::initSubRes()
{
    initTableRes();widRes->setWindowIcon(QIcon(":/icon-file/course_icon.png"));widRes->setWindowTitle("Học phần đăng kí của sinh viên");
    hhhhhLayout->setContentsMargins(100,0,100,0);comSemester->setFocusPolicy(Qt::NoFocus);comSub->setFocusPolicy(Qt::NoFocus);viewRes->setFocusPolicy(Qt::NoFocus);
    hhhhhLayout->setSpacing(10);
    hhhhhLayout->addWidget(lbSemester);hhhhhLayout->addWidget(comSemester);hhhhhLayout->addWidget(lbSub);hhhhhLayout->addWidget(comSub);hhhhhLayout->addWidget(viewRes);
    vvvvvLayout->addItem(hhhhhLayout);vvvvvLayout->addWidget(tbViewRes);widRes->setFixedSize(650,500);widRes->setWindowModality(Qt::ApplicationModal);
    widRes->setLayout(vvvvvLayout);
}

void Student_Interactive_Interface::viewResClicked()
{
    QString sub = comSub->currentText(),ses = comSemester->currentText();
    if(!sub.isEmpty() && !ses.isEmpty()){
        tbRes->setFilter("subjectID='"+sub+"' AND semester='"+ses+"'");
        tbRes->select();
    }
    if(!sub.isEmpty() && ses.isEmpty()){
        tbRes->setFilter("subjectID='"+sub+"'");
        tbRes->select();
    }
    if(sub.isEmpty() && !ses.isEmpty()){
        tbRes->setFilter("semester='"+ses+"'");
    }
    if(sub.isEmpty() && ses.isEmpty()){
        initTableRes();
    }
}

void Student_Interactive_Interface::resForAll()
{
    qDebug()<<"NUMRRRRRRRRRRRRRR->"<<numR;
    QSqlQuery del,qr,qur,ql,qll;int num,numRe,indeex;
    QString subjectID,courseCode,teaid,state;
    QString semesterR = ui->comboBox->currentText();
    QDate dateStart,dateEnd,dateTest;
    if(numR == 1){
        indeex = this->indexForTableResA; this->indexForTableResA = -1;this->indexForTableResB = -1;
        courseCode = ui->tableView_registerCourse->model()->index(indeex,0,QModelIndex()).data().toString();
        dateStart = ui->tableView_registerCourse->model()->index(indeex,1,QModelIndex()).data().toDate();
        dateEnd = ui->tableView_registerCourse->model()->index(indeex,2,QModelIndex()).data().toDate();
        dateTest = ui->tableView_registerCourse->model()->index(indeex,3,QModelIndex()).data().toDate();
        num = ui->tableView_registerCourse->model()->index(indeex,5,QModelIndex()).data().toInt();
        subjectID = ui->tableView_registerCourse->model()->index(indeex,6,QModelIndex()).data().toString();
        numRe = ui->tableView_registerCourse->model()->index(indeex,7,QModelIndex()).data().toInt();
        teaid = ui->tableView_registerCourse->model()->index(indeex,8,QModelIndex()).data().toString();
    }
    if(numR == 2){
        indeex = this->indexForTableResB; this->indexForTableResB = -1; this->indexForTableResA = -1;
        courseCode = ui->tableView_registeredCourse->model()->index(indeex,0,QModelIndex()).data().toString();
        dateStart = ui->tableView_registeredCourse->model()->index(indeex,1,QModelIndex()).data().toDate();
        dateEnd = ui->tableView_registeredCourse->model()->index(indeex,2,QModelIndex()).data().toDate();
        dateTest = ui->tableView_registeredCourse->model()->index(indeex,3,QModelIndex()).data().toDate();
        num = ui->tableView_registeredCourse->model()->index(indeex,5,QModelIndex()).data().toInt();
        subjectID = ui->tableView_registeredCourse->model()->index(indeex,6,QModelIndex()).data().toString();
        numRe = ui->tableView_registeredCourse->model()->index(indeex,7,QModelIndex()).data().toInt();
        teaid = ui->tableView_registeredCourse->model()->index(indeex,8,QModelIndex()).data().toString();
        state = ui->tableView_registeredCourse->model()->index(indeex,9,QModelIndex()).data().toString();
    }

    qDebug()<<"DATE -> "<<dateStart<<" "<<dateEnd<<" "<<dateTest;
    qDebug()<<"DADM->"<<courseCode<<" "<<subjectID<<" "<<semesterR<<" "<<num<<" "<<numRe<<" "<<teaid;
    if(numR == 1){
        qr.prepare("INSERT INTO CourseHasPassed(courseID,dateStart,dateEnd,dateTest,semester,limitedQuantity,subjectID,numRes,TEACHERID,STATE) VALUES(:courseID,:dateStart,:dateEnd,:dateTest,:semester,:limited,:subID,:num,:id,:state)");
        del.prepare("DELETE FROM listCourseOffer WHERE courseID=:id");
        qr.bindValue(9,"Đang đóng");
    }
    if(numR == 2){
        qr.prepare("INSERT INTO listCourseOffer(courseID,dateStart,dateEnd,dateTest,semester,limitedQuantity,subjectID,numRes,TEACHERID) VALUES(:courseID,:dateStart,:dateEnd,:dateTest,:semester,:limited,:subID,:num,:id)");
        del.prepare("DELETE FROM CourseHasPassed WHERE courseID=:id");
        qur.prepare("DELETE FROM registerForTheCourse WHERE CourseCode=:id");
        ql.prepare("DELETE FROM registered_course WHERE courseCode=:id");
        qll.prepare("DELETE FROM detailTIMETABLE WHERE courseCode=:id");
    }

        qr.bindValue(0,courseCode);
        qr.bindValue(1,dateStart.toString("yyyy-MM-dd"));
        qr.bindValue(2,dateEnd.toString("yyyy-MM-dd"));
        qr.bindValue(3,dateTest.toString("yyyy-MM-dd"));
        qr.bindValue(4,semesterR);
        qr.bindValue(5,num);
        qr.bindValue(6,subjectID);
        qr.bindValue(7,numRe);
        qr.bindValue(8,teaid);
        del.bindValue(0,courseCode);
        qur.bindValue(0,courseCode);ql.bindValue(0,courseCode);qll.bindValue(0,courseCode);
        if(numR == 1){
            if(qr.exec() && del.exec()){indeex = -1;messageBoxOneButton("Thông báo !","Duyệt học phần thành công !","Đồng ý",250,150,80,28);}
        }
        if(numR == 2){
            bool ok;
            if(state == "Đang mở"){
                ok = messageBoxTwoButton("Thông báo !","Học phần này đang mở bạn có chắc hủy duyệt học phần này !","Đồng ý","Hủy",400,250,80,28);
                if(ok){
                    if(qr.exec() && del.exec() && qur.exec() && ql.exec()&&qll.exec()){indeex = -1;initComboBoxCourse();messageBoxOneButton("Thông báo !","Hủy duyệt học phần thành công !","Đồng ý",250,150,80,28);}
                }
            }
            else{
                if(indeex != -1){
                    ok = messageBoxTwoButton("Thông báo !","Bạn có chắc hủy duyệt học phần này !","Đồng ý","Hủy",400,250,80,28);
                    if(ok){
                        if(qr.exec() && del.exec()){indeex = -1;initComboBoxCourse();messageBoxOneButton("Thông báo !","Hủy duyệt học phần thành công !","Đồng ý",250,150,80,28);}
                    }
                }
            }
        }
        tableModelRes->select();
        tableModelCanCRes->select();
        tbCoursePassed->select();
}

void Student_Interactive_Interface::doubleClickOnTbView(int c)
{
    int a = c;QString courseIDD;
    if(dbc == 1){
        courseIDD = ui->tableView_registerCourse->model()->index(a,0,QModelIndex()).data().toString();
    }
    if(dbc == 2){
        courseIDD = ui->tableView_registeredCourse->model()->index(a,0,QModelIndex()).data().toString();
    }
    qDebug()<<"ID-> "<<courseIDD<<" "<<ui->comboBox->currentText();
    tableModelDebt->setFilter("courseId = '"+courseIDD+"' AND semester='"+ui->comboBox->currentText()+"'");
    tableModelDebt->select();
    wid->show();
}

void Student_Interactive_Interface::initComboBoxForMC()
{
    QSqlQuery qry,qury;
    ui->comboBox_subject->clear();ui->comboBox_subject->addItem("");
    ui->comboBox_yearSch->clear();ui->comboBox_yearSch->addItem("");
    if(ui->tabWidget->currentIndex() == 0){
        qry.prepare("SELECT DISTINCT PROFESSIONAL FROM Lecturer_DATABASE ORDER BY PROFESSIONAL ASC");
    }
    if(ui->tabWidget->currentIndex() == 1){
        qry.prepare("SELECT DISTINCT MAJOR FROM Student_DATABASE ORDER BY MAJOR ASC");
        qury.prepare("SELECT DISTINCT CLASSNAME FROM Student_DATABASE ORDER BY CLASSNAME ASC");
        qury.exec();
        while(qury.next()){
            ui->comboBox_yearSch->addItem(qury.value(0).toString());
        }
    }
    qry.exec();
    while(qry.next()){
        ui->comboBox_subject->addItem(qry.value(0).toString());
    }
}

void Student_Interactive_Interface::initOptionsComboBox()
{
    ui->comboBox_options->clear();
    ui->comboBox_options->addItem("");
    ui->comboBox_options->addItem("Danh sách giảng viên");
    ui->comboBox_options->addItem("Danh sách sinh viên");
    ui->comboBox_options->addItem("Danh sách công nợ");
    ui->comboBox_options->addItem("Điểm sinh viên");
    ui->comboBox_options->addItem("Học phần đăng kí");
}

void Student_Interactive_Interface::initComboBoxDebt()
{
    QSqlQuery qury;
    ui->comboBox_debt->clear();
    qury.prepare("SELECT DISTINCT semester FROM DEBT ORDER BY semester DESC");
    qury.exec();
    ui->comboBox_debt->addItem("");
    while(qury.next()){
        ui->comboBox_debt->addItem(qury.value(0).toString());
    }
}

void Student_Interactive_Interface::initComboBoxCourse()
{
    QSqlQuery qury;
    ui->comboBox_course->clear();
    qury.prepare("SELECT DISTINCT semester FROM CourseHasPassed ORDER BY semester DESC");
    qury.exec();
    ui->comboBox_course->addItem("");
    while(qury.next()){
        ui->comboBox_course->addItem(qury.value(0).toString());
    }
}

void Student_Interactive_Interface::initComboBoxDebtOfStudent()
{
    QSqlQuery qury;
    comboxYear->clear();
    qury.prepare("SELECT DISTINCT semester FROM debtOfStudent ORDER BY semester DESC");
    qury.exec();
    comboxYear->addItem("");
    while(qury.next()){
        comboxYear->addItem(qury.value(0).toString());
    }
}

void Student_Interactive_Interface::initComboBoxMark()
{
    QSqlQuery qury,ary;
    comboxSemester->clear();comboxClass->clear();
    qury.prepare("SELECT DISTINCT SCHOOL_YEAR FROM MARK_OF_STUDENTS ORDER BY SCHOOL_YEAR DESC");
    ary.prepare("SELECT DISTINCT CLASSNAME FROM MARK_OF_STUDENTS ORDER BY CLASSNAME ASC");
    qury.exec();ary.exec();
    comboxSemester->addItem("");comboxClass->addItem("");
    while(qury.next()){
        comboxSemester->addItem(qury.value(0).toString());
    }
    while(ary.next()){
        comboxClass->addItem(ary.value(0).toString());
    }
}

void Student_Interactive_Interface::initComboRes()
{
    QSqlQuery qury,qry;
    comSub->clear();comSemester->clear();
    qry.prepare("SELECT DISTINCT subjectID FROM registered_course ORDER BY subjectID ASC");
    qury.prepare("SELECT DISTINCT semester FROM registered_course ORDER BY semester DESC");
    qry.exec();qury.exec();
    comSub->addItem("");comSemester->addItem("");
    while(qry.next()){
        comSub->addItem(qry.value(0).toString());
    }
    while(qury.next()){
        comSemester->addItem(qury.value(0).toString());
    }
}

void Student_Interactive_Interface::initWidAddNewPerson()
{
    widAddNewTeacher->setWindowTitle("Thêm");widAddNewTeacher->setWindowModality(Qt::ApplicationModal);
    labelId->setFixedSize(120,28);labelName->setFixedSize(120,28);labelMajor->setFixedSize(120,28);labelCLass->setFixedSize(120,28);
    labelId->setAlignment(Qt::AlignCenter);labelName->setAlignment(Qt::AlignCenter);labelMajor->setAlignment(Qt::AlignCenter);labelCLass->setAlignment(Qt::AlignCenter);
    lineEditId->setFixedSize(100,28);lineEditName->setFixedSize(200,28);lineEditMajor->setFixedSize(200,28);lineEditClass->setFixedSize(200,28);
    createPerson->setFixedSize(100,28);widAddNewTeacher->setFixedSize(450,300);
    vvLayout->addWidget(labelId);vvLayout->addWidget(labelName);vvLayout->addWidget(labelMajor);
    vvLayout1->addWidget(lineEditId);vvLayout1->addWidget(lineEditName);vvLayout1->addWidget(lineEditMajor);
    vvLayout->addWidget(labelCLass);vvLayout1->addWidget(lineEditClass);
    hhLayout->addItem(vvLayout);hhLayout->addItem(vvLayout1);hhLayout1->addWidget(createPerson);
    createPerson->setFocusPolicy(Qt::NoFocus);

    vvLayout2->addItem(hhLayout);vvLayout2->addItem(hhLayout1);
    widAddNewTeacher->setLayout(vvLayout2);
    widAddNewTeacher->setStyleSheet("QWidget{background-color: rgb(147, 220, 220);}"
    "QLabel{font : 8pt Courier;border : none;background-color: rgb(255, 170, 0);}"
    "QPushButton{font : 8pt Courier;background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{font : 8pt Courier;background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{font : 8pt Courier;background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}"
    "QPushButton:pressed{border:1px solid rgb(162, 108, 0);outline : 0;}"
    "QLineEdit{background-color:white;font : 8pt Courier;padding-left : 10px;border-radius : 4px;border : 1px solid;}"
    "QLineEdit:hover{font : 8pt Courier;border : 1px solid rgb(255, 170, 0);}"
    "QLineEdit:focus{font : 8pt Courier;border : 1px solid rgb(255, 170, 0);}");
    widAddNewTeacher->setFont(QFont("Courier"));
}

bool Student_Interactive_Interface::checkIDIsExisted()
{
    QSqlQuery qury;
    if(ui->tabWidget->currentIndex()==0){
        qury.prepare("SELECT MSGV FROM Teacher WHERE MSGV='"+lineEditId->text()+"'");
    }
    if(ui->tabWidget->currentIndex() == 1){
        qury.prepare("SELECT MSSV FROM Student WHERE MSSV='"+lineEditId->text()+"'");
    }
    qury.exec();
    qury.first();
    if(ui->tabWidget->currentIndex() ==0){
        if(qury.value(0).toString() == lineEditId->text()){
            QToolTip::hideText();
            QPalette pal;
            pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(lineEditId->mapToGlobal(QPoint()),tr("Mã giảng viên đã tồn tại !"));
            QToolTip::setPalette(pal);
            lineEditId->setStyleSheet("color : red");
            l = true;
            return true;
        }
        l = false;
        QToolTip::hideText();lineEditId->setStyleSheet("color : black");
        return false;
    }
    if(ui->tabWidget->currentIndex() == 1){
        if(qury.value(0).toString() == lineEditId->text()){
            QToolTip::hideText();
            QPalette pal;
            pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
            QToolTip::showText(lineEditId->mapToGlobal(QPoint()),tr("Mã sinh viên đã tồn tại !"));
            QToolTip::setPalette(pal);
            lineEditId->setStyleSheet("color : red");
            l = true;
            return true;
        }
        l = false;
        QToolTip::hideText();lineEditId->setStyleSheet("color : black");
        return false;
    }
    l = false;
    return false;
}

bool Student_Interactive_Interface::checkNewTeacherIsValid()
{
    QSqlQuery qry;int x;
    if(ui->tabWidget->currentIndex()==0){x=1;};
    if(ui->tabWidget->currentIndex()==1){x=2;};
    if(x==1){
        qry.prepare("INSERT INTO Teacher(MSGV,nameTeacher,Professional) VALUES(:id,:name,:pro)");
        qry.bindValue(0,lineEditId->text());
        qry.bindValue(1,lineEditName->text());
        qry.bindValue(2,lineEditMajor->text());

        if(!checkIDIsExisted() && !lineEditMajor->text().isEmpty() && !lineEditName->text().isEmpty()){
            if(qry.exec()){
                messageBoxOneButton("Thông báo !","\t\tThêm giảng viên thành công\t\t \n\tGiảng viên đang được lưu trong danh sách\t\t\nGiảng viên tạo tài khoản và hoàn tất thông tin cơ bản\t\t\t","Đồng ý",500,250,90,28);
                tbTeacher->select();
                m = true;
                lineEditId->setStyleSheet("color : black");
                ui->tableView_teacher->clearSelection();
                return true;
            }
        }
        if(checkIDIsExisted() && !lineEditMajor->text().isEmpty() && !lineEditName->text().isEmpty()){
            messageBoxOneButton("Thông báo !","Mã giảng viên đã tồn tại !","Đồng ý",250,150,80,28);
            lineEditId->setStyleSheet("color : red");
            return false;
        }
        else{
            messageBoxOneButton("Thông báo !","Thêm giảng viên thất bại !","Đồng ý",250,150,80,28);
            lineEditId->setStyleSheet("color : red");}
        return false;
    }

    if(x==2){
        qry.prepare("INSERT INTO Student(MSSV,tenSinhVien,CLASSNAME,MAJOR) VALUES(:id,:name,:class,:pro)");
        qry.bindValue(0,lineEditId->text());
        qry.bindValue(1,lineEditName->text());
        qry.bindValue(2,lineEditClass->text());
        qry.bindValue(3,lineEditMajor->text());

        if(!checkIDIsExisted() && !lineEditMajor->text().isEmpty() && !lineEditName->text().isEmpty() && !lineEditClass->text().isEmpty()){
            if(qry.exec()){
                messageBoxOneButton("Thông báo !","\t\tThêm sinh viên thành công\t\t \n\tSinh viên đang được lưu trong danh sách\t\t\nSinh viên tạo tài khoản và hoàn tất thông tin cơ bản\t\t\t","Đồng ý",500,250,90,28);
                tbStudent->select();
                m = true;
                lineEditId->setStyleSheet("color : black");
                ui->tableView_student->clearSelection();
                return true;
            }
        }
        if(checkIDIsExisted() && !lineEditMajor->text().isEmpty() && !lineEditName->text().isEmpty()){
            messageBoxOneButton("Thông báo !","Mã sinh viên đã tồn tại !","Đồng ý",250,150,80,28);
            lineEditId->setStyleSheet("color : red");
            return false;
        }
        else{
            messageBoxOneButton("Thông báo !","Thêm sinh viên thất bại !","Đồng ý",250,150,80,28);
            lineEditId->setStyleSheet("color : red");}
        return false;
    }

    return false;
}

void Student_Interactive_Interface::initTableStudent()
{
    tbStudent->setTable("Student");
    tbStudent->setSort(0,Qt::AscendingOrder);
    tbStudent->select();
    tbViewStudent->setModel(tbStudent);
    tbViewStudent->model()->setHeaderData(0,Qt::Horizontal,"Mã sinh viên");
    tbViewStudent->model()->setHeaderData(1,Qt::Horizontal,"Họ tên");
    tbViewStudent->model()->setHeaderData(2,Qt::Horizontal,"Lớp");
    tbViewStudent->model()->setHeaderData(3,Qt::Horizontal,"Chuyên ngành");
    tbViewStudent->setColumnWidth(0,150);
    tbViewStudent->setColumnWidth(1,200);
    tbViewStudent->setColumnWidth(2,100);
    tbViewStudent->setColumnWidth(3,350);
    tbViewStudent->setFixedSize(600,400);
    tbViewStudent->setStyleSheet("QHeaderView{font:8pt Courier;border:none;border-bottom:1px solid}"
    "QHeaderView::section{border:none;background-color: rgb(240, 255, 255);border-left-color:black;border-left : 1px solid;}"
    "QTableView{font:8pt Courier;background-color: #ecec76; selection-background-color: #d0d09c; selection-color: black;}"
    "QScrollBar::handle:horizontal{boder-radius : 7px;}");
    tbViewStudent->horizontalHeader()->setSectionsClickable(false);
    tbViewStudent->verticalHeader()->hide();
    tbViewStudent->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tbViewStudent->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tbViewStudent->setFocusPolicy(Qt::NoFocus);
    tbViewStudent->setEditTriggers(QTableView::NoEditTriggers);
    tbViewStudent->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbViewStudent->setSelectionMode(QAbstractItemView::SingleSelection);
    tbViewStudent->setWindowTitle("Danh sách sinh viên");
    tbViewStudent->setWindowModality(Qt::ApplicationModal);tbViewStudent->setWindowIcon(QIcon(":/icon-file/student_icon.png"));
}

void Student_Interactive_Interface::initTableTeacher()
{
    tbTeacher->setTable("Teacher");
    tbTeacher->setSort(0,Qt::AscendingOrder);
    tbTeacher->select();
    tbViewTeacher->setModel(tbTeacher);
    tbViewTeacher->model()->setHeaderData(0,Qt::Horizontal,"Mã giảng viên");
    tbViewTeacher->model()->setHeaderData(1,Qt::Horizontal,"Họ tên");
    tbViewTeacher->model()->setHeaderData(2,Qt::Horizontal,"Chuyên ngành");
    tbViewTeacher->setColumnWidth(0,120);
    tbViewTeacher->setColumnWidth(1,200);
    tbViewTeacher->setColumnWidth(2,200);
    tbViewTeacher->setFixedSize(500,400);
    tbViewTeacher->setStyleSheet("QHeaderView{font:8pt Courier;border:none;border-bottom:1px solid}"
    "QHeaderView::section{border:none;background-color: rgb(240, 255, 255);border-left-color:black;border-left : 1px solid;}"
    "QTableView{font:8pt Courier;background-color: #ecec76; selection-background-color: #d0d09c; selection-color: black;}"
    "QScrollBar::handle:horizontal{boder-radius : 7px;}");
    tbViewTeacher->verticalHeader()->hide();
    tbViewTeacher->horizontalHeader()->setSectionsClickable(false);
    tbViewTeacher->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tbViewTeacher->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tbViewTeacher->setFocusPolicy(Qt::NoFocus);
    tbViewTeacher->setEditTriggers(QTableView::NoEditTriggers);
    tbViewTeacher->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbViewTeacher->setSelectionMode(QAbstractItemView::SingleSelection);
    tbViewTeacher->setWindowTitle("Danh sách giảng viên");
    tbViewTeacher->setWindowModality(Qt::ApplicationModal);
    tbViewTeacher->setWindowIcon(QIcon(":/icon-file/icons8-student-center-100.png"));
}

void Student_Interactive_Interface::ifComboBoxNull(int a)
{
    ui->comboBox_subject->setToolTip("Chuyên ngành");
    ui->comboBox_yearSch->setToolTip("Lớp");
    ui->comboBox_options->setToolTip("Lựa chọn khác");
}

void Student_Interactive_Interface::comboboxDebtChange()
{
    if(comboxYear->currentText().isEmpty()){
        initSubDebtTable();
    }
    else{
        tbDebtOfStudent->setFilter("semester='"+comboxYear->currentText()+"'");
        tbDebtOfStudent->select();
    }
}

void Student_Interactive_Interface::initComboBoxTimeRes()
{
    QSqlQuery qury;
    qury.prepare("SELECT DISTINCT yearOfRes FROM timeToRegis ORDER BY yearOfRes DESC");
    qury.exec();
    ui->comboBox_2->addItem("");
    while(qury.next()){
        ui->comboBox_2->addItem(qury.value(0).toString());
    }
}

void Student_Interactive_Interface::refreshMarkTable()
{
    initTableManageMarkStudent();initComboBoxMark();tbManageMarkStudent->select();
}

void Student_Interactive_Interface::viewButtonClicked()
{
    QString cla = comboxClass->currentText(),ses=comboxSemester->currentText();
    if(!cla.isEmpty() && !ses.isEmpty()){
        tbManageMarkStudent->setFilter("SCHOOL_YEAR='"+ses+"' AND CLASSNAME='"+cla+"'");
    }
    if(!cla.isEmpty() && ses.isEmpty()){
        tbManageMarkStudent->setFilter("CLASSNAME='"+cla+"'");
    }
    if(cla.isEmpty() && !ses.isEmpty()){
        tbManageMarkStudent->setFilter("SCHOOL_YEAR='"+ses+"'");
    }
    if(cla.isEmpty() && ses.isEmpty()){
        initTableManageMarkStudent();
    }
    tbManageMarkStudent->select();
}


void Student_Interactive_Interface::tableDebtDoubleClick(QModelIndex in)
{
    QSqlQuery qury,del;int stateNum;QString state,id,cid,st;
    cid = tbViewDebtOfStudent->model()->index(in.row(),0).data().toString();
    state = tbViewDebtOfStudent->model()->index(in.row(),7).data().toString();
    id = tbViewDebtOfStudent->model()->index(in.row(),5).data().toString();
    if(state=="Chưa đóng"){
        st="Đã đóng";
        bool m = messageBoxTwoButton("Thông báo !","Đánh dấu là đã đóng tiền công nợ này !","Đã đóng","Hủy",300,150,80,28);
        if(m){
            qury.prepare("UPDATE debtOfStudent SET STATE=N'"+st+"' WHERE courseCode='"+cid+"' AND studentID='"+id+"'");
            if(qury.exec()){
                messageBoxOneButton("Thông báo !","Đánh dấu thành công !","Đồng ý",250,150,80,28);
                tbDebtOfStudent->select();
            }
        }
    }
    else{
        st="Chưa đóng";
        bool m = messageBoxTwoButton("Thông báo !","Đánh dấu là chưa đóng tiền công nợ này !","Chưa đóng","Xóa",300,150,80,28);
        if(m){
            qury.prepare("UPDATE debtOfStudent SET STATE=N'"+st+"' WHERE courseCode='"+cid+"' AND studentID='"+id+"'");
            if(qury.exec()){
                messageBoxOneButton("Thông báo !","Đánh dấu thành công !","Đồng ý",250,150,80,28);
                tbDebtOfStudent->select();
            }
        }
    }

}



void Student_Interactive_Interface::clearSelection()
{
    if(dbc == 1){
        ui->tableView_registerCourse->clearSelection();
    }
    if(dbc == 2){
        ui->tableView_registeredCourse->clearSelection();
    }
}

bool Student_Interactive_Interface::checkIdDebtValid()
{
    QSqlQuery qury;
    qury.prepare("SELECT courseID FROM CourseHasPassed WHERE courseID='"+ui->lineEdit_id->text()+"'");
    qury.exec();qury.first();
    if(ui->lineEdit_id->text() == qury.value(0).toString()){
        this->nn = true;
        return true;
    }
    this->nn = false;
    return false;
}

bool Student_Interactive_Interface::checkDebtValid()
{
    QString arg1 = ui->lineEdit_debt->text();
    for (int i = 0; i < arg1.size(); ++i) {
       if(!isdigit(arg1[i].toLatin1())){
           ndebt = false;
           return false;
       }
    }
    ndebt = true;
    return true;
}

bool Student_Interactive_Interface::checkSemesterValid()
{
    QString arg1 = ui->lineEdit_semester->text();
    for (int i = 0; i < arg1.size(); ++i) {
       if(!isdigit(arg1[i].toLatin1())){
               ydebt = false;
               return false;
       }
    }
    ydebt = true;
    return true;
}

bool Student_Interactive_Interface::checkYValid()
{
    QString arg1 = ui->lineEdit_year->text();
    for (int i = 0; i < arg1.size(); ++i) {
       if(!isdigit(arg1[i].toLatin1())){
               nj = false;
               return false;
       }
    }
    nj = true;
    return true;
}

void Student_Interactive_Interface::timeResChange()
{
    if(ui->comboBox_2->currentText().isEmpty()){
        initTableTimeRes();
    }
    else{
        tbTimeRes->setFilter("yearOfRes='"+ui->comboBox_2->currentText()+"'");
        tbTimeRes->select();
    }
}

//do somthing when pushButton_xem clicked()
void Student_Interactive_Interface::on_pushButton_xem_clicked()
{
    returnCheck();
}


void Student_Interactive_Interface::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}



void Student_Interactive_Interface::on_comboBox_options_activated(int index)
{
    if (index == 1)
    {
        tbViewTeacher->close();
        tbViewTeacher->show();
    }
    if (index == 2)
    {
        tbViewStudent->close();
        tbViewStudent->show();
    }
    if(index == 3){
        this->indexOptions = 1;
        widDebtOfStudent->close();
        widDebtOfStudent->show();
    }
    if(index == 4){
        this->indexOptions = 2;
        widMarkStudent->show();
    }
    if(index == 5){
        this->indexOptions = 3;
        widRes->show();
    }
}


void Student_Interactive_Interface::on_tableView_registerCourse_clicked(const QModelIndex &index)
{
    indexForTableResB = -1;
    getIndexTableViewResA(index.row());
    ui->tableView_registeredCourse->clearSelection();
}

void Student_Interactive_Interface::on_pushButton_regis_clicked()
{
    numR = 1;
    qDebug()<<"INDEX F->"<<this->indexForTableResA;
    resForAll();
}


void Student_Interactive_Interface::on_pushButton_cancelRegis_clicked()
{
    numR = 2;
    qDebug()<<"INDEX F->"<<this->indexForTableResB;
    resForAll();
}

void Student_Interactive_Interface::on_tableView_registeredCourse_clicked(const QModelIndex &index)
{
    indexForTableResA = -1;
    getIndexTableViewResB(index.row());
    ui->tableView_registerCourse->clearSelection();
}

void Student_Interactive_Interface::UpdateTime()
{
    qDebug()<<"DATE CURT->"<<QDate::currentDate().dayOfWeek();
    if(QDate::currentDate().dayOfWeek()==1){ti="Thứ hai,";}
    if(QDate::currentDate().dayOfWeek()==2){ti="Thứ ba,";}
    if(QDate::currentDate().dayOfWeek()==3){ti="Thứ tư,";}
    if(QDate::currentDate().dayOfWeek()==4){ti="Thứ năm,";}
    if(QDate::currentDate().dayOfWeek()==5){ti="Thứ sáu,";}
    if(QDate::currentDate().dayOfWeek()==6){ti="Thứ bảy,";}
    if(QDate::currentDate().dayOfWeek()==7){ti="Chủ nhật,";}
    ui->label_12->setText(ti+QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
}

void Student_Interactive_Interface::getIndexTableViewResA(int a)
{
    this->indexForTableResA = a;
}

void Student_Interactive_Interface::getIndexTableViewResB(int a)
{
    this->indexForTableResB = a;
}

void Student_Interactive_Interface::detailCourse()
{
    tableVieww->horizontalHeader()->setSectionsClickable(false);
    tableVieww->setWordWrap(true);
    tableVieww->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tableVieww->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tableVieww->horizontalScrollBar()->show();
    tableVieww->verticalScrollBar()->show();
    tableVieww->setFocusPolicy(Qt::NoFocus);
    tableVieww->setStyleSheet("QHeaderView{font:8pt Courier;border:none;border-bottom:1px solid}"
    "QHeaderView::section{font:8pt Courier;border:none;background-color: rgb(240, 255, 255);border-left-color:black;border-left : 1px solid;}"
    "QTableView{font:8pt Courier;background-color: #ecec76; selection-background-color: #d0d09c; selection-color: black;}"
    "QScrollBar::handle:horizontal{boder-radius : 7px;}");
    QLabel* label = new QLabel();
    QVBoxLayout* vLayout = new QVBoxLayout();
    tableModelDebt->select();
    tableVieww->setModel(tableModelDebt);
    vLayout->addWidget(label);
    vLayout->addWidget(tableVieww);
    label->setText("Danh sách chi tiết ngày học");
    wid->setLayout(vLayout);
    wid->setFixedWidth(640);
    wid->setFixedHeight(500);
    wid->setWindowTitle("Chi tiết học phần");
    QPixmap pixmap(":/icon-file/debt_student_standard.png");
    QIcon icon(pixmap);
    wid->setWindowIcon(icon);
    wid->setStyleSheet("font : 8pt Courier");
    tableVieww->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    tableVieww->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableVieww->verticalHeader()->hide();
    tableModelDebt->setHeaderData(0,Qt::Horizontal,tr("Mã học phần"));
    tableModelDebt->setHeaderData(1,Qt::Horizontal,tr("Ngày học"));
    tableModelDebt->setHeaderData(2,Qt::Horizontal,tr("Phòng học"));
    tableModelDebt->setHeaderData(3,Qt::Horizontal,tr("Ca học"));
    tableModelDebt->setHeaderData(4,Qt::Horizontal,tr("Mã môn học"));
    tableModelDebt->setHeaderData(5,Qt::Horizontal,tr("Năm học"));
    tableVieww->setColumnWidth(0,150);
    tableVieww->setColumnWidth(1,100);
    tableVieww->setColumnWidth(2,100);
    tableVieww->setColumnWidth(3,100);
    tableVieww->setColumnWidth(4,100);
    tableVieww->setColumnWidth(5,100);
    tableModelDebt->sort(1,Qt::AscendingOrder);
    wid->setWindowModality(Qt::ApplicationModal);
}


void Student_Interactive_Interface::initComboBox()
{
    ui->comboBox->clear();
    QSqlQuery qury,qry;
    qry.prepare("EXEC _tempYear");qry.exec();
    qury.prepare("SELECT DISTINCT semester FROM yearTemp ORDER BY semester DESC");
    qury.exec();
    while (qury.next()) {
        ui->comboBox->addItem(qury.value(0).toString());
    }
}


void Student_Interactive_Interface::showRowChange()
{
    for(int i = 0; i < tableModel->rowCount(); ++i)
    {
        ui->tableView_registerCourse->showRow(i);
    }
}

void Student_Interactive_Interface::showWidChangeInfor()
{
    QString id,name,major,classs;
    if(tbA != -1 && ui->tabWidget->currentIndex() == 0){
        id = ui->tableView_teacher->model()->index(tbA,0,QModelIndex()).data().toString();
        name = ui->tableView_teacher->model()->index(tbA,1,QModelIndex()).data().toString();
        major = ui->tableView_teacher->model()->index(tbA,2,QModelIndex()).data().toString();
    }
    if(tbB != -1 && ui->tabWidget->currentIndex() == 1){
        id = ui->tableView_student->model()->index(tbB,0,QModelIndex()).data().toString();
        name = ui->tableView_student->model()->index(tbB,1,QModelIndex()).data().toString();
        classs = ui->tableView_student->model()->index(tbB,2,QModelIndex()).data().toString();
        major = ui->tableView_student->model()->index(tbB,3,QModelIndex()).data().toString();
    }
    if(ui->tabWidget->currentIndex() == 0){updateInformation->setB(1);};
    if(ui->tabWidget->currentIndex() == 1){updateInformation->setB(2);updateInformation->setLClass(classs);};
    updateInformation->setOldId(id);
    updateInformation->setLId(id);updateInformation->setLName(name);updateInformation->setLMajor(major);
    updateInformation->show();
}

void Student_Interactive_Interface::showUpConTextMenu(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);
          menu->addAction(new QAction("Thêm giảng viên", this));
          menu->addAction(new QAction("Xóa giảng viên", this));
          menu->addAction(new QAction("Chỉnh sửa", this));
          menu->addAction(new QAction("In danh sách", this));
          menu->addAction(new QAction("Làm mới", this));
          menu->popup(ui->tableView_teacher->viewport()->mapToGlobal(pos));
      QObject::connect(menu->actions().at(0),&QAction::triggered,this,&Student_Interactive_Interface::addTeacher);
      QObject::connect(menu->actions().at(1),&QAction::triggered,this,&Student_Interactive_Interface::delTeacher);
      QObject::connect(menu->actions().at(2),&QAction::triggered,this,&Student_Interactive_Interface::editTeacher);
      QObject::connect(menu->actions().at(3),&QAction::triggered,this,&Student_Interactive_Interface::printListTeacherAndStudent);
      QObject::connect(menu->actions().at(4),&QAction::triggered,this,&Student_Interactive_Interface::initTableManageTeacher);
      QPoint point;
      point = menu->mapToParent (pos);
      menu->show();
      qDebug() << "pos: " << pos;
}

void Student_Interactive_Interface::showUpConTextMenu1(const QPoint &pos)
{
    QMenu* menu1 = new QMenu(this);
          menu1->addAction(new QAction("Thêm sinh viên", this));
          menu1->addAction(new QAction("Xóa sinh viên", this));
          menu1->addAction(new QAction("Chỉnh sửa", this));
          menu1->addAction(new QAction("In danh sách", this));
          menu1->addAction(new QAction("Làm mới", this));
          menu1->popup(ui->tableView_student->viewport()->mapToGlobal(pos));
      QObject::connect(menu1->actions().at(0),&QAction::triggered,this,&Student_Interactive_Interface::addTeacher);
      QObject::connect(menu1->actions().at(1),&QAction::triggered,this,&Student_Interactive_Interface::delTeacher);
      QObject::connect(menu1->actions().at(2),&QAction::triggered,this,&Student_Interactive_Interface::editTeacher);
      QObject::connect(menu1->actions().at(3),&QAction::triggered,this,&Student_Interactive_Interface::printListTeacherAndStudent);
      QObject::connect(menu1->actions().at(4),&QAction::triggered,this,&Student_Interactive_Interface::initTableManageStudent);
      QPoint point;
      point = menu1->mapToParent (pos);
      menu1->show();
      qDebug() << "pos: " << pos;
}

void Student_Interactive_Interface::showUpConTextMenu2(const QPoint &pos)
{
    QMenu* menu2 = new QMenu(this);
          menu2->addAction(new QAction("Làm mới", this));
          menu2->popup(ui->tableView_registerCourse->viewport()->mapToGlobal(pos));
      QObject::connect(menu2->actions().at(0),&QAction::triggered,this,&Student_Interactive_Interface::doFreshTbListCourse);
      QPoint point;
      point = menu2->mapToParent (pos);
      menu2->show();
      qDebug() << "pos: " << pos;
}

void Student_Interactive_Interface::showUpConTextMenu3(const QPoint &pos)
{
    QMenu* menu1 = new QMenu(this);
    menu1->setStyleSheet("background-color:white;border-radius:7px");
    menu1->addAction(new QAction("In danh sách điểm", this));
    menu1->addAction(new QAction("Làm mới", this));
    menu1->popup(tbViewMarkStudent->viewport()->mapToGlobal(pos));
    QObject::connect(menu1->actions().at(0),&QAction::triggered,this,&Student_Interactive_Interface::prinListDebtMarkRes);
    QObject::connect(menu1->actions().at(1),&QAction::triggered,this,&Student_Interactive_Interface::refreshMarkTable);
    QPoint point;
    point = menu1->mapToParent (pos);
    menu1->show();
    qDebug() << "pos: " << pos;
}

void Student_Interactive_Interface::showUpConTextMenu4(const QPoint &pos)
{
    QMenu* menu1 = new QMenu(this);
    menu1->setStyleSheet("background-color:white;border-radius:7px");
    menu1->addAction(new QAction("In công nợ", this));
    menu1->addAction(new QAction("Làm mới", this));
    menu1->popup(tbViewDebtOfStudent->viewport()->mapToGlobal(pos));
    QObject::connect(menu1->actions().at(0),&QAction::triggered,this,&Student_Interactive_Interface::prinListDebtMarkRes);
    QObject::connect(menu1->actions().at(1),&QAction::triggered,this,&Student_Interactive_Interface::refreshDebtRes);
    QPoint point;
    point = menu1->mapToParent (pos);
    menu1->show();
    qDebug() << "pos: " << pos;
}

void Student_Interactive_Interface::showUpConTextMenu6(const QPoint &pos)
{
    QMenu* menu1 = new QMenu(this);
    menu1->setStyleSheet("background-color:white;border-radius:7px");
    menu1->addAction(new QAction("In danh sách", this));
    menu1->addAction(new QAction("Làm mới", this));
    menu1->popup(tbViewRes->viewport()->mapToGlobal(pos));
    QObject::connect(menu1->actions().at(0),&QAction::triggered,this,&Student_Interactive_Interface::prinListDebtMarkRes);
    QObject::connect(menu1->actions().at(1),&QAction::triggered,this,&Student_Interactive_Interface::refreshDebtRes);
    QPoint point;
    point = menu1->mapToParent (pos);
    menu1->show();
    qDebug() << "pos: " << pos;
}

void Student_Interactive_Interface::refreshDebtRes()
{
    if(indexOptions==1){tbDebtOfStudent->select();initComboBoxDebtOfStudent();}
    if(indexOptions==3){tbRes->select();initComboRes();}
}

void Student_Interactive_Interface::doubleClickedOnTbStudent(const QModelIndex &index)
{
    QSqlQuery qury;QString ar;
    ar = tbViewStudent->model()->index(index.row(),0).data().toString();
    bool c = messageBoxTwoButton("Thông báo !","Thông tin liên quan đến sinh viên này sẽ bị xóa !","Đồng ý","Hủy",350,150,80,28);
    if(c){
        qury.prepare("EXEC _deleteAllInforStudent '"+ar+"'");
        if(qury.exec()){
            messageBoxOneButton("Thông báo !","Xóa toàn bộ thông tin thành công !","Đồng ý",250,150,80,28);
            initComboBoxForMC();
            initTableManageStudent();
            initTableStudent();
            initSubDebtTable();
            initTableManageMarkStudent();
            initComboBoxMark();
        }
        else{
            messageBoxOneButton("Thông báo !","Xóa toàn bộ thông tin thất bại !","Đồng ý",250,150,80,28);
        }
    }
}

void Student_Interactive_Interface::doubleClickedOnTbTeacher(const QModelIndex &index)
{
    QSqlQuery qury;QString ar;
    ar = tbViewTeacher->model()->index(index.row(),0).data().toString();
    bool c = messageBoxTwoButton("Thông báo !","Thông tin liên quan đến giảng viên này sẽ bị xóa !","Đồng ý","Hủy",350,150,80,28);
    if(c){
        qury.prepare("EXEC _deleteAllInforTeacher '"+ar+"'");
        if(qury.exec()){
            messageBoxOneButton("Thông báo !","Xóa toàn bộ thông tin thành công !","Đồng ý",250,150,80,28);
            initComboBox();
            initComboBoxForMC();
            initOptionsComboBox();
            initTableManageTeacher();
            initTableManageCourse();
            initTableManageCoursePassed();

            initTableTeacher();
            initTableCoursePassed();
            initComboBoxCourse();
        }
        else{
            messageBoxOneButton("Thông báo !","Xóa toàn bộ thông tin thất bại !","Đồng ý",250,150,80,28);
        }
    }
}

void Student_Interactive_Interface::textLineEditMajorChange()
{
    QString tt = lineEditMajor->text().toLower();
    tt[0]=tt.at(0).toUpper();
    lineEditMajor->setText(tt);
    qDebug()<<"UPPER->"<<tt;
}

void Student_Interactive_Interface::textLineEditIDChange()
{
    QString tt = lineEditId->text().toUpper();
    lineEditId->setText(tt);
    qDebug()<<"UPPER->"<<tt;
}

void Student_Interactive_Interface::textLineEditNameChange()
{
    QString tt = lineEditName->text().toLower();
    tt[0]=tt.at(0).toUpper();
    lineEditName->setText(tt);
    qDebug()<<"UPPER->"<<tt;
}

void Student_Interactive_Interface::refreshListCourse()
{
    tableModelRes->setFilter("semester='"+ui->comboBox->currentText()+"'");
    tableModelCanCRes->setFilter("semester='"+ui->comboBox->currentText()+"'");
    tableModelRes->select();
    tableModelCanCRes->select();
}



void Student_Interactive_Interface::printListTeacherAndStudent()
{
    QXlsx::Document xlxs;int x,y;QString value;
    Format format,formatCellHeader,formatCell;

    format.setFontBold(true);format.setFontColor(QColor(0,0,0));
    format.setFontName("Calibri");format.setHorizontalAlignment(Format::AlignHCenter);format.setFontSize(18);
    format.setPatternBackgroundColor(QColor(255,170,0));format.setVerticalAlignment(Format::AlignVCenter);

    formatCellHeader.setFontName("Calibri");formatCellHeader.setPatternBackgroundColor(QColor(255,170,0));formatCellHeader.setFontBold(false);
    formatCellHeader.setFontSize(13);formatCellHeader.setBorderStyle(Format::BorderThin);formatCellHeader.setBorderColor(QColor(0,0,0));
    formatCellHeader.setHorizontalAlignment(Format::AlignHCenter);formatCellHeader.setVerticalAlignment(Format::AlignVCenter);formatCell.setTextWrap(true);

    formatCell.setFontName("Calibri");formatCell.setPatternBackgroundColor(QColor(196,215,155));
    formatCell.setFontSize(13);formatCell.setBorderStyle(Format::BorderThin);formatCell.setBorderStyle(Format::BorderThin);
    formatCell.setHorizontalAlignment(Format::AlignHCenter);formatCell.setVerticalAlignment(Format::AlignVCenter);

    xlxs.mergeCells(CellRange("E3:G3"));
    xlxs.setRowHeight(3,30);
    xlxs.setRowHeight(6,20);
    if(indexTabWidget==0){
        xlxs.write("E3","DANH SÁCH GIẢNG VIÊN",format);
        xlxs.write("A6","Mã giảng viên",formatCellHeader);xlxs.write("B6","Họ tên",formatCellHeader);xlxs.write("C6","Chuyên ngành",formatCellHeader);
        xlxs.write("D6","Căn cước công dân",formatCellHeader);xlxs.write("E6","Số điện thoại",formatCellHeader);xlxs.write("F6","Email",formatCellHeader);xlxs.write("G6","Địa chỉ",formatCellHeader);
        xlxs.write("H6","Giới tính",formatCellHeader);xlxs.write("I6","Ngày sinh",formatCellHeader);xlxs.write("J6","Dân tộc",formatCellHeader);xlxs.write("K6","Nơi sinh",formatCellHeader);
        xlxs.setColumnWidth(1,20);
        xlxs.setColumnWidth(2,40);
        xlxs.setColumnWidth(3,60);
        xlxs.setColumnWidth(4,20);
        xlxs.setColumnWidth(5,20);
        xlxs.setColumnWidth(6,40);
        xlxs.setColumnWidth(7,60);
        xlxs.setColumnWidth(8,20);
        xlxs.setColumnWidth(9,20);
        xlxs.setColumnWidth(10,20);
        xlxs.setColumnWidth(11,50);
        x = ui->tableView_teacher->model()->rowCount();y=ui->tableView_teacher->model()->columnCount();
    }
    if(indexTabWidget==1){
        xlxs.write("E3","DANH SÁCH SINH VIÊN",format);
        xlxs.write("A6","Mã sinh viên",formatCellHeader);xlxs.write("B6","Họ tên",formatCellHeader);xlxs.write("C6","Lớp",formatCellHeader);
        xlxs.write("D6","Chuyên ngành",formatCellHeader);xlxs.write("E6","Email",formatCellHeader);xlxs.write("F6","Giới tính",formatCellHeader);xlxs.write("G6","Số điện thoại",formatCellHeader);
        xlxs.write("H6","Địa chỉ",formatCellHeader);xlxs.write("I6","Căn cước công dân",formatCellHeader);xlxs.write("J6","Ngày sinh",formatCellHeader);xlxs.write("K6","Dân tộc",formatCellHeader);xlxs.write("L6","Nơi sinh",formatCellHeader);
        xlxs.setColumnWidth(1,20);
        xlxs.setColumnWidth(2,40);
        xlxs.setColumnWidth(3,20);
        xlxs.setColumnWidth(4,50);
        xlxs.setColumnWidth(5,40);
        xlxs.setColumnWidth(6,20);
        xlxs.setColumnWidth(7,30);
        xlxs.setColumnWidth(8,60);
        xlxs.setColumnWidth(9,30);
        xlxs.setColumnWidth(10,20);
        xlxs.setColumnWidth(11,20);
        xlxs.setColumnWidth(12,50);
        x = ui->tableView_student->model()->rowCount();y=ui->tableView_student->model()->columnCount();
    }

    if(indexTabWidget==0){
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {

                value = ui->tableView_teacher->model()->index(i,j,QModelIndex()).data().toString();
                if(j < 9){
                    xlxs.write(i+7,j+1,value,formatCell);
                }
                if(j>10){
                    xlxs.write(i+7,j-1,value,formatCell);
                }
            }
        }
    }
    if(indexTabWidget==1){
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                value = ui->tableView_student->model()->index(i,j,QModelIndex()).data().toString();
                if(j < 8){
                    xlxs.write(i+7,j+1,value,formatCell);
                }
                if(j>9){
                    xlxs.write(i+7,j-1,value,formatCell);
                }
            }
        }
    }

    QString setFill = QString("Excel (*.xlsx)");
    QString fileNamePath = QFileDialog::getSaveFileName(this,"Lưu file","",setFill);
    bool saveCheck = xlxs.saveAs(fileNamePath);
    if(saveCheck){
        if(indexTabWidget==0){messageBoxOneButton("Thông báo !","Xuất danh sách giảng viên thành công !","Đồng ý",300,150,80,28);}
        if(indexTabWidget==1){messageBoxOneButton("Thông báo !","Xuất danh sách sinh viên thành công !","Đồng ý",300,150,80,28);}
    }
    else{
        messageBoxOneButton("Thông báo !","Thất bại ! File đang được sử dụng nên không thể xuất !","Đồng ý",300,150,80,28);}
}



void Student_Interactive_Interface::prinListDebtMarkRes()
{
    QXlsx::Document xlxs;QString value,nn;
    Format format,formatCellHeader,formatCell;

    format.setFontBold(true);format.setFontColor(QColor(0,0,0));
    format.setFontName("Calibri");format.setHorizontalAlignment(Format::AlignHCenter);format.setFontSize(18);
    format.setPatternBackgroundColor(QColor(255,170,0));format.setVerticalAlignment(Format::AlignVCenter);

    formatCellHeader.setFontName("Calibri");formatCellHeader.setPatternBackgroundColor(QColor(255,170,0));formatCellHeader.setFontBold(false);
    formatCellHeader.setFontSize(13);formatCellHeader.setBorderStyle(Format::BorderThin);formatCellHeader.setBorderColor(QColor(0,0,0));
    formatCellHeader.setHorizontalAlignment(Format::AlignHCenter);formatCellHeader.setVerticalAlignment(Format::AlignVCenter);formatCell.setTextWrap(true);

    formatCell.setFontName("Calibri");formatCell.setPatternBackgroundColor(QColor(196,215,155));
    formatCell.setFontSize(13);formatCell.setBorderStyle(Format::BorderThin);formatCell.setBorderStyle(Format::BorderThin);
    formatCell.setHorizontalAlignment(Format::AlignHCenter);formatCell.setVerticalAlignment(Format::AlignVCenter);

    xlxs.mergeCells(CellRange("E3:G3"));
    xlxs.setRowHeight(3,30);
    xlxs.setRowHeight(6,20);
    if(indexOptions==1){
        nn="Xuất danh sách công nợ thành công";
        xlxs.write("E3","DANH SÁCH CÔNG NỢ",format);
        xlxs.write("B6","Mã môn học",formatCellHeader);xlxs.write("C6","Tên môn học",formatCellHeader);
        xlxs.write("D6","Công nợ",formatCellHeader);xlxs.write("E6","Số tín chỉ",formatCellHeader);xlxs.write("F6","Tổng công nợ",formatCellHeader);
        xlxs.write("G6","Mã sinh viên",formatCellHeader);xlxs.write("H6","Học kì",formatCellHeader);xlxs.write("I6","Trạng thái",formatCellHeader);
        xlxs.setColumnWidth(2,20);xlxs.setColumnWidth(3,20);xlxs.setColumnWidth(4,15);xlxs.setColumnWidth(5,20);
        xlxs.setColumnWidth(6,20);xlxs.setColumnWidth(7,30);xlxs.setColumnWidth(8,20);xlxs.setColumnWidth(9,30);
        for (int i = 0; i < tbViewDebtOfStudent->model()->rowCount(); ++i) {
            for (int j = 0; j < tbViewDebtOfStudent->model()->columnCount(); ++j) {
                value = tbViewDebtOfStudent->model()->index(i,j,QModelIndex()).data().toString();
                xlxs.write(i+7,j+2,value,formatCell);
            }
        }

    }
    if(indexOptions==2){
        nn="Xuất danh sách điểm sinh viên thành công";
        xlxs.write("E3","DANH SÁCH ĐIỂM SINH VIÊN",format);
        xlxs.write("A6","Mã sinh viên",formatCellHeader);xlxs.write("B6","Mã môn học",formatCellHeader);xlxs.write("C6","Năm học",formatCellHeader);
        xlxs.write("D6","Lớp",formatCellHeader);xlxs.write("E6","Điểm lần 1",formatCellHeader);xlxs.write("F6","Điểm lần 2",formatCellHeader);xlxs.write("G6","Điểm lần 3",formatCellHeader);
        xlxs.write("H6","Thường xuyên",formatCellHeader);xlxs.write("I6","Cuối kì",formatCellHeader);xlxs.write("J6","Tổng kết",formatCellHeader);xlxs.write("K6","Điểm 4",formatCellHeader);xlxs.write("L6","Điểm chữ",formatCellHeader);xlxs.write("M6","Xếp loại",formatCellHeader);
        xlxs.setColumnWidth(1,30);xlxs.setColumnWidth(2,20);xlxs.setColumnWidth(3,20);xlxs.setColumnWidth(4,20);xlxs.setColumnWidth(5,20);xlxs.setColumnWidth(12,20);xlxs.setColumnWidth(13,20);
        xlxs.setColumnWidth(6,20);xlxs.setColumnWidth(7,20);xlxs.setColumnWidth(8,20);xlxs.setColumnWidth(9,20);xlxs.setColumnWidth(10,20);xlxs.setColumnWidth(11,20);
        for (int i = 0; i < tbViewMarkStudent->model()->rowCount(); ++i) {
            for (int j = 0; j < tbViewMarkStudent->model()->columnCount(); ++j) {
                value = tbViewMarkStudent->model()->index(i,j,QModelIndex()).data().toString();
                xlxs.write(i+7,j+1,value,formatCell);
            }
        }
    }
    if(indexOptions==3){
        nn="Xuất danh sách sinh viên đăng kí học phần thành công";
        xlxs.write("E3","DANH SÁCH SINH VIÊN ĐĂNG KÍ HỌC PHẦN",format);
        xlxs.write("B6","Mã học phần",formatCellHeader);xlxs.write("C6","Mã môn học",formatCellHeader);
        xlxs.write("D6","Ngày bắt đầu",formatCellHeader);xlxs.write("E6","Ngày kết thúc",formatCellHeader);xlxs.write("F6","Học kì",formatCellHeader);xlxs.write("G6","Mã sinh viên",formatCellHeader);
        xlxs.write("H6","Giảng viên",formatCellHeader);xlxs.write("I6","Mã giảng viên",formatCellHeader);xlxs.write("J6","Số tín chỉ",formatCellHeader);
        xlxs.setColumnWidth(2,20);xlxs.setColumnWidth(3,20);xlxs.setColumnWidth(4,20);xlxs.setColumnWidth(5,20);
        xlxs.setColumnWidth(6,20);xlxs.setColumnWidth(7,30);xlxs.setColumnWidth(8,30);xlxs.setColumnWidth(9,20);
        for (int i = 0; i < tbViewRes->model()->rowCount(); ++i) {
            for (int j = 0; j < tbViewRes->model()->columnCount(); ++j) {
                value = tbViewRes->model()->index(i,j,QModelIndex()).data().toString();
                xlxs.write(i+7,j+2,value,formatCell);
            }
        }
    }

    QString setFill = QString("Excel (*.xlsx)");
    QString fileNamePath = QFileDialog::getSaveFileName(this,"Lưu file","",setFill);
    bool saveCheck = xlxs.saveAs(fileNamePath);
    if(saveCheck){
        if(indexOptions==1){messageBoxOneButton("Thông báo !",nn,"Đồng ý",300,150,80,28);}
        if(indexOptions==2){messageBoxOneButton("Thông báo !",nn,"Đồng ý",300,150,80,28);}
        if(indexOptions==3){messageBoxOneButton("Thông báo !",nn,"Đồng ý",300,150,80,28);}
    }
    else{
        messageBoxOneButton("Thông báo !","Thất bại ! File đang được sử dụng nên không thể xuất !","Đồng ý",300,150,80,28);}
}



void Student_Interactive_Interface::mChange()
{
    m = false;
}



void Student_Interactive_Interface::delTeacher(int ac)
{
    QSqlQuery qr,qury;bool c;QString ar;
    if(ui->tabWidget->currentIndex() == 0){
        ar = ui->tableView_teacher->model()->index(tbA,0,QModelIndex()).data().toString();
        qDebug()<<"PPA->"<<tbA<<" "<<ar;
        qr.prepare("DELETE FROM Lecturer_DATABASE WHERE MSGV='"+ar+"'");
        //qury.prepare("DELETE FROM Teacher WHERE MSGV='"+ar+"'");
        if(tbA != -1){c = messageBoxTwoButton("Thông báo !","Bạn có chắc xóa giảng viên này không ?","Đồng ý","Hủy",250,150,80,28);}
        if(c && tbA != -1){if(qr.exec()){messageBoxOneButton("Thông báo !","Xóa giảng viên thành công !","Đồng ý",250,150,80,28);this->tbA = -1;ui->tableView_teacher->clearSelection();}}
        else{this->tbA = -1;ui->tableView_teacher->clearSelection();}//tbTeacher->select();
    }
    if(ui->tabWidget->currentIndex() == 1){
        ar = ui->tableView_student->model()->index(tbB,0,QModelIndex()).data().toString();
        qDebug()<<"PPB->"<<tbB<<" "<<ar;
        qr.prepare("DELETE FROM Student_DATABASE WHERE MSSV='"+ar+"'");
        //qury.prepare("DELETE FROM Student WHERE MSSV='"+ar+"'");
        if(tbB != -1){c = messageBoxTwoButton("Thông báo !","Bạn có chắc xóa sinh viên này không ?","Đồng ý","Hủy",250,150,80,28);}
        if(c && tbB != -1){if(qr.exec()){messageBoxOneButton("Thông báo !","Xóa sinh viên thành công !","Đồng ý",250,150,80,28);this->tbB = -1;ui->tableView_student->clearSelection();}}
        else{this->tbB = -1;ui->tableView_student->clearSelection();}//tbStudent->select();
    }
    returnCheck();
}

void Student_Interactive_Interface::addTeacher()
{
    widAddNewTeacher->close();
    lineEditId->clear();lineEditMajor->clear();lineEditName->clear();lineEditClass->clear();
    if(ui->tabWidget->currentWidget() == 0){
        labelCLass->hide();lineEditClass->hide();
    }
    if(ui->tabWidget->currentIndex()==1){labelCLass->show();lineEditClass->show();}
    widAddNewTeacher->show();
}

void Student_Interactive_Interface::editTeacher(int ac)
{
    if(tbA != -1 && ui->tabWidget->currentIndex() == 0){
        updateInformation->setB(1);
        updateInformation->hideLineEditClass();
        showWidChangeInfor();
    }
    else{
        ui->tableView_teacher->clearSelection();
    }
    if(tbB != -1 && ui->tabWidget->currentIndex() == 1){
        updateInformation->setB(2);
        updateInformation->showLineEditClass();
        showWidChangeInfor();
    }
    else{
        ui->tableView_student->clearSelection();
    }
}

void Student_Interactive_Interface::freshTable()
{
    returnCheck();
}

void Student_Interactive_Interface::idTrue()
{
    QSqlQuery qury;
    qury.prepare("SELECT semester FROM CourseHasPassed WHERE courseID='"+ui->lineEdit_id->text()+"'");
    if(checkIdDebtValid()){
        qury.exec();qury.first();
        ui->lineEdit_semester->setText(qury.value(0).toString());
    }
}

void Student_Interactive_Interface::doFreshTbListCourse()
{
    initComboBox();
    refreshListCourse();
}


void Student_Interactive_Interface::closeEvent(QCloseEvent *event)
{
    if(checkClose){
        bool check = messageBoxTwoButton("Thông báo !","Bạn có chắc muốn thoát chương trình !","Đồng ý","Hủy",250,150,80,28);
        if(check){event->accept();qApp->exit();}else{event->ignore();}
    }
}

void Student_Interactive_Interface::returnCheck()
{
    QString sub,classd;
    sub = ui->comboBox_subject->currentText();
    qDebug()<<"SUBJECT ->"<<ui->comboBox_subject->currentText()<<" "<<ui->comboBox_yearSch->currentText();
    if(ui->tabWidget->currentIndex() == 0){
        if(!ui->comboBox_subject->currentText().isEmpty()){
            tableModel->setQuery("SELECT * FROM Lecturer_DATABASE WHERE PROFESSIONAL=N'"+sub+"'");
            ui->tableView_teacher->setModel(tableModel);
        }
        if(ui->comboBox_subject->currentText().isEmpty()){
            tableModel->setQuery("SELECT * FROM Lecturer_DATABASE");
            ui->tableView_teacher->setModel(tableModel);
        }
        tbTeacher->select();
        tbA = -1;
        ui->tableView_teacher->clearSelection();
    }

    if(ui->tabWidget->currentIndex() == 1){
        sub = ui->comboBox_subject->currentText();
        classd = ui->comboBox_yearSch->currentText();

        if(!ui->comboBox_subject->currentText().isEmpty() && !ui->comboBox_yearSch->currentText().isEmpty()){
            tb->setQuery("SELECT * FROM Student_DATABASE WHERE CLASSNAME='"+classd+"' AND MAJOR=N'"+sub+"'");
            ui->tableView_student->setModel(tb);
        }
        if(!ui->comboBox_subject->currentText().isEmpty() && ui->comboBox_yearSch->currentText().isEmpty()){
            tb->setQuery("SELECT * FROM Student_DATABASE WHERE MAJOR=N'"+sub+"'");
            ui->tableView_student->setModel(tb);
        }
        if(ui->comboBox_subject->currentText().isEmpty() && !ui->comboBox_yearSch->currentText().isEmpty()){
            tb->setQuery("SELECT * FROM Student_DATABASE WHERE CLASSNAME='"+classd+"'");
            ui->tableView_student->setModel(tb);
        }
        if(ui->comboBox_subject->currentText().isEmpty() && ui->comboBox_yearSch->currentText().isEmpty()){
            tb->setQuery("SELECT * FROM Student_DATABASE");
            ui->tableView_student->setModel(tb);
        }
        tbStudent->select();
        tbB = -1;
        ui->tableView_student->clearSelection();
    }
}


void Student_Interactive_Interface::insertToRes(int ls)
{
    QSqlQuery qury,qury1,qry,qry1,ql;bool l = false;
    QString id,dateS,dateE,dateT,se,numLi,subId,num,teId,sta;
    id = ui->tableView_registeredCourse->model()->index(ls,0,QModelIndex()).data().toString();
    dateS = ui->tableView_registeredCourse->model()->index(ls,1,QModelIndex()).data().toString();
    dateE = ui->tableView_registeredCourse->model()->index(ls,2,QModelIndex()).data().toString();
    dateT = ui->tableView_registeredCourse->model()->index(ls,3,QModelIndex()).data().toString();
    se = ui->tableView_registeredCourse->model()->index(ls,4,QModelIndex()).data().toString();
    numLi = ui->tableView_registeredCourse->model()->index(ls,5,QModelIndex()).data().toString();
    subId = ui->tableView_registeredCourse->model()->index(ls,6,QModelIndex()).data().toString();
    num = ui->tableView_registeredCourse->model()->index(ls,7,QModelIndex()).data().toString();
    teId = ui->tableView_registeredCourse->model()->index(ls,8,QModelIndex()).data().toString();
    sta = ui->tableView_registeredCourse->model()->index(ls,9,QModelIndex()).data().toString();
    qury.prepare("SELECT TENMONHOC FROM SUBJECT_ WHERE MAMONHOC='"+subId+"'");
    qry.prepare("SELECT nameTeacher FROM Teacher WHERE MSGV='"+teId+"'");
    ql.prepare("INSERT INTO detailTIMETABLE(courseCode,dateStudy,studyTime,classroom,TeacherId)"
                "SELECT courseId,dateStudy,session,room,teacherID FROM scheduleTableOffer WHERE scheduleTableOffer.courseId='"+id+"'");
    //qry1.prepare("SELECT courseCode FROM registerForTheCourse WHERE courseCode='"+id+"'");
    qury.exec();qury.first();qry.exec();qry.first();//qry1.exec();qry1.first();
    if(qry1.value(0).toString() == id){l = true;}else{l=false;}
    if(sta=="Đang đóng"){
        qury1.prepare("EXEC _insertCourseToRes '"+id+"',N'"+qury.value(0).toString()+"','"+dateS+"','"+dateE+"','"+se+"','"+numLi+"',N'"+qry.value(0).toString()+"','"+teId+"','"+num+"'");
        if(!id.isEmpty() && ls >= 0){
            if(qury1.exec() && ql.exec()){
                messageBoxOneButton("Thông báo !","Mở học phần thành công","Đồng ý",250,150,80,28);
                tableModelCanCRes->select();indexForTableResB=-1;
                tbCoursePassed->select();
                initComboBoxCourse();
            }
        }
        ls = -1;
    }
    else{
        messageBoxOneButton("Thông báo !","Thất bại ! Học phần đã được mở !","Đồng ý",250,150,80,28);
        ui->tableView_registeredCourse->clearSelection();
    }
}



void Student_Interactive_Interface::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
    {
        this->indexTabWidget = 0;
        ui->verticalSpacerL->changeSize(40,540);
        ui->verticalWidget->show();
        ui->tableView_timeRes->hide();
        ui->label_18->hide();ui->label_19->hide();
        ui->frame->hide();
        ui->label_10->hide();
        ui->comboBox_yearSch->hide();
        initComboBoxForMC();
        tbB = -1;
        labelCLass->hide();lineEditClass->hide();
        updateInformation->setB(1);
        initOptionsComboBox();
    }
    if(index == 1)
    {
        this->indexTabWidget = 1;
        ui->verticalSpacerL->changeSize(40,540);
        ui->verticalWidget->show();
        ui->tableView_timeRes->hide();
        ui->label_18->hide();ui->label_19->hide();
        ui->frame->hide();
        ui->label_10->show();
        ui->comboBox_yearSch->show();
        initComboBoxForMC();
        tbA = -1;
        labelCLass->show();lineEditClass->show();
        updateInformation->setB(2);
        initOptionsComboBox();
    }
    if(index == 2)
    {
        ui->verticalWidget->hide();
        ui->tableView_timeRes->show();
        ui->label_18->show();ui->label_19->show();
        ui->frame->show();
        initOptionsComboBox();
        ui->verticalSpacerP->changeSize(40,100);
        ui->verticalSpacerL->changeSize(40,300);
    }
    if(index == 3)
    {
        ui->verticalWidget->hide();
        ui->tableView_timeRes->show();
        ui->label_18->show();ui->label_19->show();
        ui->frame->show();
        initOptionsComboBox();
        ui->verticalSpacerP->changeSize(40,100);
        ui->verticalSpacerL->changeSize(40,300);
    }
}

void Student_Interactive_Interface::on_pushButton_teacher_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab_teacher);
}


void Student_Interactive_Interface::on_pushButton_student_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab_student);
}

void Student_Interactive_Interface::on_tableView_registerCourse_doubleClicked(const QModelIndex &index)
{
    dbc = 1;doubleClickOnTbView(index.row());
}


void Student_Interactive_Interface::on_tableView_registeredCourse_doubleClicked(const QModelIndex &index)
{
    bool doo;
    doo = messageBoxTwoButton("Thông báo !","\tĐồng ý mở học phần này !\t","Đồng ý","Hủy",300,150,80,28);
    if(doo){
        insertToRes(index.row());
    }
    else{
        this->indexForTableResB = -1;
        ui->tableView_registeredCourse->clearSelection();
    }
}


void Student_Interactive_Interface::on_tableView_teacher_clicked(const QModelIndex &index)
{
    this->tbA = index.row();
}


void Student_Interactive_Interface::on_tableView_student_clicked(const QModelIndex &index)
{
    this->tbB = index.row();
}


void Student_Interactive_Interface::on_pushButton_updateInfor_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}


void Student_Interactive_Interface::on_lineEdit_id_textChanged(const QString &arg1)
{
    if(!checkIdDebtValid()){
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_id->mapToGlobal(QPoint()),tr("Mã học phần không đúng !"));
        QToolTip::setPalette(pal);
        ui->lineEdit_id->setStyleSheet("color : red");
    }
    else{
        QToolTip::hideText();
        ui->lineEdit_id->setStyleSheet("color : black");
    }

}


void Student_Interactive_Interface::on_lineEdit_debt_textChanged(const QString &arg1)
{

       if(!checkDebtValid()){
           QToolTip::hideText();
           QPalette pal;
           pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
           QToolTip::showText(ui->lineEdit_debt->mapToGlobal(QPoint()),tr("Công nợ không đúng !"));
           QToolTip::setPalette(pal);
           ui->lineEdit_debt->setStyleSheet("color : red");
           ndebt = false;
       }
       else{
           ui->lineEdit_debt->setStyleSheet("color : black");
           ndebt = true;
       }
}


void Student_Interactive_Interface::on_lineEdit_semester_textChanged(const QString &arg1)
{
       if(!checkSemesterValid()){
               QToolTip::hideText();
               QPalette pal;
               pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
               QToolTip::showText(ui->lineEdit_semester->mapToGlobal(QPoint()),tr("Năm học không đúng !"));
               QToolTip::setPalette(pal);
               ui->lineEdit_semester->setStyleSheet("color : red");
               ydebt = false;
       }
       else{
           ui->lineEdit_semester->setStyleSheet("color : black");
           ydebt = true;
       }
}


void Student_Interactive_Interface::on_pushButton_createDebt_clicked()
{
    qDebug()<<"TEXT->"<<ui->lineEdit_id->text()<<" "<<ui->lineEdit_debt->text()<<" "<<ui->lineEdit_semester->text();
    QSqlQuery qury,qry,q;bool n = false;
    qury.prepare("SELECT courseCode,semester FROM DEBT WHERE courseCode='"+ui->lineEdit_id->text()+"' AND semester='"+ui->lineEdit_semester->text()+"'");
    qury.exec();qury.first();
    if(!qury.value(0).toString().isEmpty()){
        n = true;
        messageBoxOneButton("Thông báo !","Công nợ đã tồn tại !","Đồng ý",250,150,80,28);
    }
    if(nn && ndebt && ydebt && !n ){
        n = true;
        q.prepare("SELECT subjectID FROM CourseHasPassed WHERE courseID='"+ui->lineEdit_id->text()+"'");
        q.exec();q.first();
        qDebug()<<"imDD->>"<<q.value(0).toString();
        qry.prepare("INSERT INTO DEBT(courseCode,subjectName,debt,semester) VALUES('"+ui->lineEdit_id->text()+"','"+q.value(0).toString()+"','"+ui->lineEdit_debt->text()+"','"+ui->lineEdit_semester->text()+"')");
        if(qry.exec()){
            messageBoxOneButton("Thông báo !","Thêm công nợ thành công !","Đồng ý",250,150,80,28);
            initComboBoxDebt();
            tbDebt->select();
        }
    }
    if(!n){
        qDebug()<<"AA";
        messageBoxOneButton("Thông báo !","Thêm công nợ thất bại !","Đồng ý",250,150,80,28);
    }
    qDebug()<<"LMMMMM->"<<nn<<" "<<ndebt<<" "<<ydebt<<" "<<n;
}


void Student_Interactive_Interface::on_tableView_listDebt_doubleClicked(const QModelIndex &index)
{
    ui->tableView_openCourse->clearSelection();
    bool l = messageBoxTwoButton("Xóa công nợ","Xóa công nợ này khỏi danh sách !","Đồng ý","Hủy",300,150,80,28);
    if(l){
        messageBoxOneButton("Thông báo !","Xóa thành công !","Đồng ý",250,150,80,28);
        ui->tableView_listDebt->model()->removeRow(index.row(),QModelIndex());
        tbDebt->select();
    }
}


void Student_Interactive_Interface::on_comboBox_debt_currentIndexChanged(const QString &arg1)
{
    if(arg1.isEmpty()){
        initTableDebt();
    }
    else{
        tbDebt->setFilter("semester='"+arg1+"'");
        tbDebt->setSort(0,Qt::AscendingOrder);
        tbDebt->select();
    }
}


void Student_Interactive_Interface::on_comboBox_course_currentIndexChanged(const QString &arg1)
{
    if(arg1.isEmpty()){
        initTableCoursePassed();
    }
    else{
        tbCoursePassed->setFilter("semester='"+arg1+"'");
        tbCoursePassed->setSort(0,Qt::AscendingOrder);
        tbCoursePassed->select();
    }
}


void Student_Interactive_Interface::on_tableView_openCourse_clicked(const QModelIndex &index)
{
    ui->tableView_listDebt->clearSelection();
    ui->tableView_timeRes->clearSelection();
    sl = index.row();
}


void Student_Interactive_Interface::on_tableView_listDebt_clicked(const QModelIndex &index)
{
    sl = -1;
    ui->tableView_openCourse->clearSelection();
    ui->tableView_timeRes->clearSelection();
}


void Student_Interactive_Interface::on_lineEdit_year_textChanged(const QString &arg1)
{
    if(!checkYValid()){
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_year->mapToGlobal(QPoint()),tr("Năm không hợp lệ !"));
        QToolTip::setPalette(pal);
        ui->lineEdit_year->setStyleSheet("color : red");
    }
    else{
        QToolTip::hideText();
        ui->lineEdit_year->setStyleSheet("color : black");
    }

}


void Student_Interactive_Interface::on_lineEdit_timeRes_textChanged(const QString &arg1)
{
    string dater= ui->lineEdit_timeRes->text().toStdString();
    for (unsigned long long i = 0;i < dater.size() ;++i ) {
        if (dater[i]=='/') {
            dater.replace(i,1,"-");
        }
    }
    ui->lineEdit_timeRes->setText(dater.c_str());
    QDate date =QDate::fromString(ui->lineEdit_timeRes->text(),"dd-MM-yyyy");
    if (!date.isValid())
    {
        QToolTip::hideText();
        ui->lineEdit_timeRes->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_timeRes->mapToGlobal(QPoint()),tr("Ngày không hợp lệ !"));
        QToolTip::setPalette(pal);
        mj = false;
    }
    if (date.isValid())
    {
        ui->lineEdit_timeRes->setStyleSheet("color : black");
        mj =true;
    }
}


void Student_Interactive_Interface::on_pushButton_clicked()
{
    QSqlQuery qury,qry;QString date_birth;
    QDate date = QDate::fromString(ui->lineEdit_timeRes->text(),"dd-MM-yyyy");
    date_birth = date.toString("yyyy-MM-dd");
    qury.prepare("INSERT INTO timeToRegis(yearOfRes,timeOverRes) VALUES(:dataa,CAST(:dateb AS DATE))");
    qry.prepare("SELECT yearOfRes FROM timeToRegis WHERE yearOfRes='"+ui->lineEdit_year->text()+"'");
    qry.exec();qry.first();
    qury.bindValue(0,ui->lineEdit_year->text());
    qury.bindValue(1,date_birth);
    if(nj &&  mj){
        if(qry.value(0).toString()!=ui->lineEdit_year->text()){
            if(qury.exec() && nj && mj){
                messageBoxOneButton("Thông báo !","Thêm thời gian thành công !","Đồng ý",250,150,80,28);
                tbTimeRes->select();
            }
        }
        else{
            messageBoxOneButton("Thông báo !","Thời gian này đã tồn tại !","Đồng ý",250,150,80,28);
        }
    }
    else if(((!nj && mj))||(nj && !mj)){
        messageBoxOneButton("Thông báo !","Nhập thông tin không hợp lệ ! ","Đồng ý",250,150,80,28);
    }
    else if((!nj && !mj && !ui->lineEdit_timeRes->text().isEmpty()) && !ui->lineEdit_year->text().isEmpty()){
        messageBoxOneButton("Thông báo !","Nhập thông tin không hợp lệ !","Đồng ý",250,150,80,28);
    }
    else{
        messageBoxOneButton("Thông báo !","Vui lòng nhập đầy đủ thông tin !","Đồng ý",250,150,80,28);
    }
}







void Student_Interactive_Interface::on_tableView_timeRes_doubleClicked(const QModelIndex &index)
{
    bool k = messageBoxTwoButton("Thông báo !","Bạn có chắc xóa thời hạn này !","Đồng ý","Hủy",400,250,80,28);
    if(k){
        messageBoxOneButton("Thông báo !","Xóa thời hạn thành công !","Đồng ý",250,150,80,28);
        ui->tableView_timeRes->model()->removeRow(index.row());
        tbTimeRes->select();
    }
}


void Student_Interactive_Interface::on_tableView_timeRes_clicked(const QModelIndex &index)
{
    sl = -1;
    ui->tableView_listDebt->clearSelection();
    ui->tableView_openCourse->clearSelection();
}


void Student_Interactive_Interface::on_actionTh_ng_tin_sinh_vi_n_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_teacher);
}


void Student_Interactive_Interface::on_actionK_t_qua_ho_c_t_p_triggered()
{
    ui->tabWidget->setCurrentWidget(ui->tab_student);
}


void Student_Interactive_Interface::on_actionLi_ch_ho_c_triggered()
{
    ui->tabWidget->setCurrentIndex(2);
}


void Student_Interactive_Interface::on_actionLi_ch_thi_triggered()
{
    ui->tabWidget->setCurrentIndex(3);
}


void Student_Interactive_Interface::on_actionThoa_t_triggered()
{
    this->indexOptions = 1;
    widDebtOfStudent->close();
    widDebtOfStudent->show();
}


void Student_Interactive_Interface::on_action_i_m_t_kh_u_triggered()
{
    this->indexOptions = 2;
    widMarkStudent->show();
}


void Student_Interactive_Interface::on_actionKh_i_ng_l_i_triggered()
{
    this->indexOptions = 3;
    widRes->show();
}


void Student_Interactive_Interface::on_actionThoa_t_2_triggered()
{
    this->close();
}


