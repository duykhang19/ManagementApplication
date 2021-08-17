#include "ChangeMarkOfStudent.h"
#include "ui_ChangeMarkOfStudent.h"
#include<QMainWindow>

using namespace std;


ChangeMarkOfStudent::ChangeMarkOfStudent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeMarkOfStudent)
{
    ui->setupUi(this);
    Init();
    initMapGrading(setMap);
    //initComboxBox();
    this->index = 0;
    this->setWindowModality(Qt::NonModal);
    QObject::connect(ui->lineEdit_mssv,&QLineEdit::textChanged,this,&ChangeMarkOfStudent::initComboxBox);
}

ChangeMarkOfStudent::~ChangeMarkOfStudent()
{
    delete ui;
}


void ChangeMarkOfStudent::on_lineEdit_mssv_editingFinished()
{
    editingFinishStuId();
    //initComboxBox();
}


void ChangeMarkOfStudent::Init()
{
    this->check = false;
    ui->lineEdit_mssv->setFocus();
    ui->lineEdit->setMaxLength(4);
    ui->lineEdit_2->setMaxLength(10);
    ui->lineEdit_mssv->setMaxLength(10);
    ui->lineEdit_lan1->setMaxLength(4);
    ui->lineEdit_lan2->setMaxLength(4);
    ui->lineEdit_lan3->setMaxLength(4);
    ui->lineEdit_thuongxuyen->setMaxLength(4);
    ui->lineEdit_cuoiki->setMaxLength(4);
    ui->lineEdit_tongket->setMaxLength(4);
    ui->lineEdit_thangd4->setMaxLength(4);
    ui->lineEdit_diemchu->setMaxLength(2);
    ui->lineEdit_xeploai->setMaxLength(30);
    ui->comboBox_year->setFocusPolicy(Qt::NoFocus);
}

void ChangeMarkOfStudent::on_lineEdit_lan1_editingFinished()
{
    checkTest1A();
}

bool ChangeMarkOfStudent::checkMarkValid(QString demo)
{
    int a = 0;
    string dem = demo.toStdString();
    for (unsigned long long i =0;i <= dem.size()-1 ;i++ ) {
        if (dem[0]=='.') {
            return false;
        }
        if (!isdigit(dem[i]) && dem[i] != '.')
        {
            return false;
        }
        if (dem[i]=='.') {
            ++a;
        }
    }
    if (a==2){return false;}
    double data = demo.toDouble();
    if (data>=0 && data<=10)
    {
        return true;
    }
    else{return false;}

    return true;
}

void ChangeMarkOfStudent::on_lineEdit_lan2_editingFinished()
{
    checkTest2A();
}

void ChangeMarkOfStudent::on_lineEdit_lan3_editingFinished()
{
    checkTest3A();
}

void ChangeMarkOfStudent::on_lineEdit_thuongxuyen_editingFinished()
{
    if (checkMarkValid(ui->lineEdit_thuongxuyen->text()))
    {
        ui->lineEdit_thuongxuyen->setStyleSheet("color : black");
        QToolTip::hideText();
    }
    if(!checkMarkValid(ui->lineEdit_thuongxuyen->text()))
    {
        QToolTip::hideText();
        ui->lineEdit_thuongxuyen->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_thuongxuyen->mapToGlobal(QPoint()),tr("Điểm thi không hợp lệ !"));
        QToolTip::setPalette(pal);
    }
}

void ChangeMarkOfStudent::on_lineEdit_cuoiki_editingFinished()
{
    checkFinalPointA();
}

void ChangeMarkOfStudent::on_pushButton_capnhat_clicked()
{
    bool checkAnnounce = false;
    qDebug()<<getIndex();
    ui->lineEdit_mssv->setStyleSheet("color:black");ui->lineEdit_2->setStyleSheet("color:black");ui->lineEdit->setStyleSheet("color:black");
    if (getIndex() == 1) {
        if (!check || !checkTest1 || !checkTest2 || !checkTest3 || !checkEnd || ui->comboBox_year->currentText().isEmpty())
        {
            if (!check){ui->lineEdit_mssv->setStyleSheet("color:red");}
            checkAnnounce = true;
            messageBoxOneButton("Thông báo","Hãy kiểm tra và sửa những vùng nhập sai hoặc thiếu !","Đồng ý",270,150,80,25);
        }
    }
    if (getIndex() == 2) {
        if (!check || !checkTest1 || !checkTest2 || !checkTest3 || !checkEnd || ui->lineEdit_2->text().isEmpty() || ui->lineEdit->text().isEmpty())
        {
            if(!check){ui->lineEdit_mssv->setStyleSheet("color:red");ui->lineEdit->setStyleSheet("color:red");ui->lineEdit_2->setStyleSheet("color:red");}
            checkAnnounce = true;
            messageBoxOneButton("Thông báo","Hãy kiểm tra và sửa những vùng nhập sai hoặc thiếu !","Đồng ý",270,150,80,25);
        }
    }
    if (check && checkTest1 && checkTest2 && checkTest3 && checkEnd && !checkIsNotNull())// && !ui->comboBox_year->currentText().isEmpty())
    {
        if (getIndex() == 1 && !checkAnnounce){
            if(!checkValidObject()){messageBoxOneButton("Thông báo !","Sinh viên này chưa có điểm nên không thể cập nhật !","Đồng ý",200,150,90,28);}
            else if(!checkValidObject() || !checkHadRes()){messageBoxOneButton("Thông báo !","Sinh viên này chưa đăng kí học phần này !","Đồng ý",450,150,90,25);ui->lineEdit_mssv->setStyleSheet("color : red");}
        }
        if (getIndex() == 2 && !checkAnnounce ){
            if(!checkValidObject() && !checkHadRes()){messageBoxOneButton("Thông báo !","Sinh viên này chưa đăng kí học phần này !","Đồng ý",450,150,90,25);ui->lineEdit_mssv->setStyleSheet("color : red");ui->lineEdit->setStyleSheet("color : red");ui->lineEdit_2->setStyleSheet("color:red");}
            else if (!checkValidObject()){messageBoxOneButton("Thông báo !","Sinh viên đã có điểm trong học phần này !","Đồng ý",450,150,90,25);ui->lineEdit_mssv->setStyleSheet("color : red");ui->lineEdit->setStyleSheet("color : red");ui->lineEdit_2->setStyleSheet("color:red");}
            else if(!checkHadRes()){messageBoxOneButton("Thông báo !","Sinh viên chưa đăng kí học phần này !","Đồng ý",450,150,90,25);ui->lineEdit_mssv->setStyleSheet("color : red");ui->lineEdit->setStyleSheet("color : red");ui->lineEdit_2->setStyleSheet("color:red");}
        }
        if (checkValidObject() && checkHadRes()){
            QSqlQuery qry;
            if (getIndex() == 1 && !ui->comboBox_year->currentText().isEmpty()) {
                qry.prepare("UPDATE MARK_OF_STUDENTS"
                            " SET kiemTraLan1 = :test1,kiemTraLan2 = :test2, kiemTraLan3 = :test3,DIEMTHUONGXUYEN= :thuongxuyen,DIEMCUOIKI = :cuoiki,DIEMTONGKET= :tongket,THANGDIEM4= :thangdiem4,DIEMCHU= :diemchu,XEPLOAI= :xeploai"
                            " WHERE MSSV= :mssv AND MAMONHOC= :mamonhoc AND SCHOOL_YEAR= :year_sch");
            }
            if (getIndex() == 2 && !ui->lineEdit_2->text().isEmpty() && !ui->lineEdit->text().isEmpty()) {
                qry.prepare("INSERT INTO MARK_OF_STUDENTS(kiemTraLan1,kiemTraLan2,kiemTraLan3,DIEMTHUONGXUYEN,DIEMCUOIKI,DIEMTONGKET,THANGDIEM4,DIEMCHU,XEPLOAI,MSSV,MAMONHOC,SCHOOL_YEAR,CLASSNAME)"
                            " VALUES(:kiemtralan1,:kiemtralan2,:kiemtralan3,:thuongxuyen,:cuoiki,:tongket,:thangdiem4,:diemchu,:xeploai,:mssv,:mamonhoc,:schoolyear,:classname)");
                qry.bindValue(12,ui->lineEdit_2->text());
            }
            qry.bindValue(0,ui->lineEdit_lan1->text());
            qry.bindValue(1,ui->lineEdit_lan2->text());
            qry.bindValue(2,ui->lineEdit_lan3->text());
            qry.bindValue(3,ui->lineEdit_thuongxuyen->text());
            qry.bindValue(4,ui->lineEdit_cuoiki->text());
            qry.bindValue(5,ui->lineEdit_tongket->text());
            qry.bindValue(6,ui->lineEdit_thangd4->text());
            qry.bindValue(7,ui->lineEdit_diemchu->text());
            qry.bindValue(8,ui->lineEdit_xeploai->text());
            qry.bindValue(9,ui->lineEdit_mssv->text());
            qry.bindValue(10,getPro());
            if(getIndex() == 1){qry.bindValue(11,ui->comboBox_year->currentText());}
            if(getIndex() == 2){qry.bindValue(11,ui->lineEdit->text());}
            if (qry.exec()) {
                if(getIndex() == 1){messageBoxOneButton("Thông báo !","Cập nhật thành công !","Đồng ý",300,150,80,25);}
                if(getIndex() == 2){ui->lineEdit_mssv->setStyleSheet("color : black");ui->lineEdit->setStyleSheet("color : black");ui->lineEdit_2->setStyleSheet("color:black");messageBoxOneButton("Thông báo !","Thêm sinh viên thành công !","Đồng ý",300,150,80,25);}
                emit destroyed();
            }
        }
    }
}

void ChangeMarkOfStudent::setPro(QString a)
{
    this->pro = a;
}

QString ChangeMarkOfStudent::getPro(){return this->pro;}
double  ChangeMarkOfStudent::calculateProcessScore(double a, double b, double c)
{
    return (a + b + c)/(double)3;
}

QString ChangeMarkOfStudent::getStudentGrading(double mark)
{
    if(mark>=(double)9.0 && mark <=(double)10){return "A+";}
    if(mark>=(double)8.5 && mark<(double)9.0){return "A";}
    if(mark>=(double)8.0 && mark<(double)8.5){return "B+";}
    if(mark>=(double)7.0 && mark<(double)8.0){return "B";}
    if(mark>=(double)6.5 && mark<(double)7.0){return "C+";}
    if(mark>=(double)5.5 && mark<(double)6.5){return "C";}
    if(mark>=(double)5.0 && mark<(double)5.5){return "D+";}
    if(mark>=(double)4.0 && mark<(double)5.0){return "D";}
    if(mark>=(double)0.0 && mark<(double)4.0){return "F";}
    return "Lỗi";
}

double ChangeMarkOfStudent::calculateFinalGrade(double a, double b)
{
    return ((a*(double)0.3) + (b*(double)0.7));
}

double::ChangeMarkOfStudent::calculateFourGradeScale(double a)
{
    return (a/(double)10)*4;
}
void ChangeMarkOfStudent::doWhenTextChanged()
{
    double test1,test2,test3,end;
    if(checkTest1){test1 = ui->lineEdit_lan1->text().toDouble();}else{test1 = 0;};
    if(checkTest2){test2 = ui->lineEdit_lan2->text().toDouble();}else{test2 = 0;};
    if(checkTest3){test3 = ui->lineEdit_lan3->text().toDouble();}else{test3 = 0;};
    if (checkEnd){end = ui->lineEdit_cuoiki->text().toDouble();}else{end=0;};
    if(check && checkTest1 && checkTest2 && checkTest3 && checkEnd)
    {
        solveMark(test1,test2,test3,end);
    }
    else
    {
        solveMark(test1,test2,test3,end);
    }
}

void ChangeMarkOfStudent::initMapGrading(QMap<QString,QString>& setMap)
{
    setMap.insert("A+","Xuất xắc");
    setMap.insert("A","Giỏi");
    setMap.insert("B+","Khá giỏi");
    setMap.insert("B","Khá");
    setMap.insert("C+","Trung bình khá");
    setMap.insert("C","Trung bình");
    setMap.insert("D+","Trung bình yếu");
    setMap.insert("D","Trung bình");
    setMap.insert("F","Yếu");
}

bool ChangeMarkOfStudent::checkIsNotNull()
{
    if (ui->lineEdit_mssv->text().isEmpty()){return true;}
    if(ui->lineEdit_lan1->text().isEmpty()){return true;}
    if(ui->lineEdit_lan2->text().isEmpty()){return true;}
    if(ui->lineEdit_lan3->text().isEmpty()){return true;}
    if(ui->lineEdit_cuoiki->text().isEmpty()){return true;}
    return false;
}

void ChangeMarkOfStudent::checkTest1A()
{
    if (checkMarkValid(ui->lineEdit_lan1->text()))
    {
        ui->lineEdit_lan1->setStyleSheet("color : black");
        QToolTip::hideText();
        this->checkTest1 = true;
    }
    if(!checkMarkValid(ui->lineEdit_lan1->text()))
    {
        QToolTip::hideText();
        ui->lineEdit_lan1->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_lan1->mapToGlobal(QPoint()),tr("Điểm thi không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->checkTest1 = false;
    }
    doWhenTextChanged();
}

void ChangeMarkOfStudent::checkTest2A()
{
    if (checkMarkValid(ui->lineEdit_lan2->text()))
    {
        ui->lineEdit_lan2->setStyleSheet("color : black");
        QToolTip::hideText();
        this->checkTest2 = true;
    }
    if(!checkMarkValid(ui->lineEdit_lan2->text()))
    {
        QToolTip::hideText();
        ui->lineEdit_lan2->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_lan2->mapToGlobal(QPoint()),tr("Điểm thi không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->checkTest2 = false;
    }
    doWhenTextChanged();
}

void ChangeMarkOfStudent::checkTest3A()
{
    if (checkMarkValid(ui->lineEdit_lan3->text()))
    {
        ui->lineEdit_lan3->setStyleSheet("color : black");
        QToolTip::hideText();
        this->checkTest3 = true;
    }
    if(!checkMarkValid(ui->lineEdit_lan3->text()))
    {
        QToolTip::hideText();
        ui->lineEdit_lan3->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_lan3->mapToGlobal(QPoint()),tr("Điểm thi không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->checkTest3 = false;
    }
    doWhenTextChanged();
}

void ChangeMarkOfStudent::checkFinalPointA()
{
    if (checkMarkValid(ui->lineEdit_cuoiki->text()))
    {
        ui->lineEdit_cuoiki->setStyleSheet("color : black");
        QToolTip::hideText();
        this->checkEnd = true;
    }
    if(!checkMarkValid(ui->lineEdit_cuoiki->text()))
    {
        QToolTip::hideText();
        ui->lineEdit_cuoiki->setStyleSheet("color : red");
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_cuoiki->mapToGlobal(QPoint()),tr("Điểm thi không hợp lệ !"));
        QToolTip::setPalette(pal);
        this->checkEnd = false;
    }
    doWhenTextChanged();
}

bool ChangeMarkOfStudent::checkStudentId()
{
   QString stuId = ui->lineEdit_mssv->text();
   QSqlQuery qury;
   if(getIndex() == 1){qury.prepare("SELECT DISTINCT MSSV FROM MARK_OF_STUDENTS WHERE MAMONHOC='"+getPro()+"' AND MSSV=:mssv");}
   if(getIndex() == 2){qury.prepare("SELECT DISTINCT studentID FROM registered_course WHERE subjectID='"+getPro()+"' AND studentID=:mssv");}
   qury.bindValue(0,stuId);
   qury.exec();
   qury.first();
   if (stuId == qury.value(0).toString()){return true;}
   return false;
}

void ChangeMarkOfStudent::on_lineEdit_mssv_textChanged(const QString &arg1)
{
    editingFinishStuId();
    //initComboxBox();
}

void ChangeMarkOfStudent::on_lineEdit_lan1_textChanged(const QString &arg1)
{
    checkTest1A();
}

void ChangeMarkOfStudent::on_lineEdit_lan2_textChanged(const QString &arg1)
{
    checkTest2A();
}

void ChangeMarkOfStudent::on_lineEdit_lan3_textChanged(const QString &arg1)
{
    checkTest3A();
}

void ChangeMarkOfStudent::on_lineEdit_cuoiki_textChanged(const QString &arg1)
{
    checkFinalPointA();
}

void ChangeMarkOfStudent::solveMark(double a, double b, double c,double d)
{
    QMap<QString,QString>::iterator iter;
    stringstream ss;
    double endPoint;
    double finalPoint = d;
    QString textQstring;
    string text;
    ss<<calculateProcessScore(a,b,c);
    ss>>text;
    textQstring=text.c_str();
    ui->lineEdit_thuongxuyen->setText(textQstring);
    endPoint=calculateFinalGrade(calculateProcessScore(a,b,c),finalPoint);
    qDebug()<<endPoint;
    ss.clear();text.clear();textQstring.clear();
    ss<<endPoint;
    ss>>text;
    textQstring=text.c_str();
    ui->lineEdit_tongket->setText(textQstring);
    double fourPoint = calculateFourGradeScale(endPoint);
    ss.clear();text.clear();textQstring.clear();
    ss<<fourPoint;
    ss>>text;
    textQstring=text.c_str();
    ui->lineEdit_thangd4->setText(textQstring);
    ui->lineEdit_diemchu->setText(getStudentGrading(endPoint));
    for (iter = setMap.begin();iter != setMap.end();++iter ) {
        if (getStudentGrading(endPoint) == iter.key()) {
            ui->lineEdit_xeploai->setText(iter.value());
        }
    }
}

void ChangeMarkOfStudent::editingFinishStuId()
{
    qDebug()<<"index : "<<getIndex();

    if (!checkStudentId() && getIndex() == 1)
    {
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_mssv->mapToGlobal(QPoint()),tr("Sinh viên này chưa có điểm nên không thể cập nhật !"));
        QToolTip::setPalette(pal);
        ui->lineEdit_mssv->setStyleSheet("color : red");
        this->check = false;
    }

    if(!checkHadRes() && getIndex() == 1)
    {
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        QToolTip::showText(ui->lineEdit_mssv->mapToGlobal(QPoint()),tr("Sinh viên này chưa đăng kí học phần này !"));
        QToolTip::setPalette(pal);
        ui->lineEdit_mssv->setStyleSheet("color : red");
        this->check = false;
    }

    if (checkStudentId() && getIndex() == 1) {
        QToolTip::hideText();
        ui->lineEdit_mssv->setStyleSheet("color : black");
        this->check = true;
    }
    if (!checkStudentId() && getIndex() == 2)
    {
        QToolTip::hideText();
        QPalette pal;
        pal.setColor(QPalette::Inactive,QPalette::ToolTipText,0xa00000);
        ui->lineEdit_mssv->setStyleSheet("color : red");
        QToolTip::showText(ui->lineEdit_mssv->mapToGlobal(QPoint()),tr("Sinh viên này không nằm trong lớp của bạn !"));
        QToolTip::setPalette(pal);
        this->check = false;
        //this->check = true;
    }
    if (checkStudentId() && getIndex() == 2) {
        QToolTip::hideText();
        ui->lineEdit_mssv->setStyleSheet("color : black");
        ui->lineEdit->setStyleSheet("color:black");
        ui->lineEdit_2->setStyleSheet("color:black");
        this->check = true;
    }
}

void ChangeMarkOfStudent::on_pushButton_clicked()
{
    clearData();
}

void ChangeMarkOfStudent::initComboxBox()
{
    QSqlQuery qury,qry;
    if(getIndex() == 1){
        ui->comboBox_year->clear();
        qry.prepare("SELECT MAMONHOC FROM SUBJECT_ WHERE TENMONHOC =:tenmobhoc");
        qry.bindValue(0,getSubId());
        qry.exec();
        qry.first();
        qDebug()<<qry.value(0).toString();
        //qury.prepare("SELECT DISTINCT SCHOOL_YEAR FROM MARK_OF_STUDENTS WHERE MSSV =:mssv AND MAMONHOC =:mamonhoc");
        qury.prepare("SELECT DISTINCT semester FROM registered_course WHERE studentID =:mssv AND subjectID =:mamonhoc");
        qury.bindValue(0,ui->lineEdit_mssv->text());
        qury.bindValue(1,qry.value(0).toString());
        qury.exec();
        while (qury.next()){
            if (!qury.value(0).toString().isEmpty()) {
                  ui->comboBox_year->addItem(qury.value(0).toString());
            }
        }
        ui->comboBox_year->setCurrentText(ui->comboBox_year->itemText(ui->comboBox_year->count()-1));
    }
    if (getIndex() == 2) {
        ui->lineEdit_2->clear();
        qury.prepare("SELECT CLASSNAME FROM Student WHERE MSSV=:mssv");
        qury.bindValue(0,ui->lineEdit_mssv->text());
        qury.exec();
        qury.first();
        ui->lineEdit_2->setText(qury.value(0).toString());
    }
}

void ChangeMarkOfStudent::setSubId(QString ms){this->subId = ms;}

QString ChangeMarkOfStudent::getSubId(){return this->subId;}

void ChangeMarkOfStudent::setStudentMark(QString mssv, QString year, QString lan1, QString lan2, QString lan3, QString quatrinh, QString cuoiki, QString tongket, QString thangdiem4, QString diemchu, QString xeploai)
{
    ui->lineEdit_mssv->setText(mssv);
    ui->comboBox_year->setCurrentText(year);
    ui->lineEdit_lan1->setText(lan1);
    ui->lineEdit_lan2->setText(lan2);
    ui->lineEdit_lan3->setText(lan3);
    ui->lineEdit_thuongxuyen->setText(quatrinh);
    ui->lineEdit_cuoiki->setText(cuoiki);
    ui->lineEdit_tongket->setText(tongket);
    ui->lineEdit_thangd4->setText(thangdiem4);
    ui->lineEdit_diemchu->setText(diemchu);
    ui->lineEdit_xeploai->setText(xeploai);
}

void ChangeMarkOfStudent::clearData()
{
    ui->lineEdit_mssv->clear();
    ui->lineEdit_lan1->clear();
    ui->lineEdit_lan2->clear();
    ui->lineEdit_lan3->clear();
    ui->lineEdit_thuongxuyen->clear();
    ui->lineEdit_cuoiki->clear();
    ui->lineEdit_tongket->clear();
    ui->lineEdit_xeploai->clear();
    ui->lineEdit_diemchu->clear();
    ui->lineEdit_thangd4->clear();
    ui->comboBox_year->currentText().clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit->setStyleSheet("color:black");
    ui->lineEdit_2->setStyleSheet("color:black");
    ui->lineEdit_mssv->setStyleSheet("color:black");
    ui->lineEdit_mssv->setFocus();
}


void ChangeMarkOfStudent::setIndex(int a){this->index = a;}
int ChangeMarkOfStudent::getIndex(){return this->index;}

void ChangeMarkOfStudent::hideLineEdit(){ui->lineEdit->hide();}
void ChangeMarkOfStudent::showLineEdit(){ui->lineEdit->show();ui->horizontalSpacer_6->changeSize(120,0);}
void ChangeMarkOfStudent::hideComboBox(){ui->comboBox_year->hide();}
void ChangeMarkOfStudent::showComboBox(){ui->comboBox_year->show();ui->horizontalSpacer_6->changeSize(65,20);}

void ChangeMarkOfStudent::hideClass()
{
    ui->label_12->hide();
    ui->lineEdit_2->hide();
    ui->pushButton_2->hide();
}

void ChangeMarkOfStudent::showClass()
{
    ui->label_12->show();
    ui->lineEdit_2->show();
    ui->pushButton_2->show();
}

void ChangeMarkOfStudent::setTextLineEdit(QString text)
{
    ui->pushButton_capnhat->setText(text);
}

bool ChangeMarkOfStudent::checkValidObject()
{
    QSqlQuery qry;
    //qry.prepare("SELECT Student.MSSV,Student.CLASSNAME,MARK_OF_STUDENTS.SCHOOL_YEAR FROM MARK_OF_STUDENTS,Student WHERE MAMONHOC=:mamonhoc AND Student.MSSV = MARK_OF_STUDENTS.MSSV");
    qry.prepare("SELECT Student.MSSV,Student.CLASSNAME,MARK_OF_STUDENTS.SCHOOL_YEAR FROM MARK_OF_STUDENTS,Student "
                " WHERE MAMONHOC=:mamonhoc AND Student.MSSV = MARK_OF_STUDENTS.MSSV AND Student.MSSV=:mssv AND MARK_OF_STUDENTS.SCHOOL_YEAR=:year");
    qry.bindValue(0,getPro());
    qry.bindValue(1,ui->lineEdit_mssv->text());
    if(getIndex() == 1){qry.bindValue(2,ui->comboBox_year->currentText());}
    if(getIndex() == 2){qry.bindValue(2,ui->lineEdit->text());}
    qry.exec();
    qry.first();
    qDebug()<<ui->lineEdit_mssv->text()<<" "<<ui->lineEdit->text()<<" "<<ui->comboBox_year->currentText();
    qDebug()<<"MAJOR : "<<getIndex()<<" "<<getPro()<<qry.value(0).toString()<<" "<<qry.value(1).toString()<<" "<<qry.value(2).toString();
    if (getIndex() == 1) {
        if (qry.value(0).toString() == ui->lineEdit_mssv->text() && qry.value(2).toString() == ui->comboBox_year->currentText()) {
            ui->lineEdit_mssv->setStyleSheet("color : black");
            qDebug()<<"đã tồn tại 1";
            return true;
        }
        else{qDebug()<<"Chưa tồn tại 1";}
    }
    if (getIndex() == 2) {
        if (qry.value(0).toString() != ui->lineEdit_mssv->text() && qry.value(2).toString() != ui->lineEdit->text()  && ui->lineEdit_2->text() != qry.value(1).toString()) {
            qDebug()<<"Điểm này chưa tồn tại 2";
            return true;
        }
        if (qry.value(0).toString() != ui->lineEdit_mssv->text() && qry.value(2).toString() != ui->lineEdit->text()  && ui->lineEdit_2->text() != qry.value(1).toString()) {
            qDebug()<<"Đã tồn tại 2";
        }
    }
    return false;
}

bool ChangeMarkOfStudent::checkHadRes()
{
    QSqlQuery qury;
    qury.prepare("SELECT studentID,subjectID,semester FROM registered_course WHERE studentID=:mssv and subjectID=:subjectId AND semester=:year");
    qury.bindValue(0,ui->lineEdit_mssv->text());
    qury.bindValue(1,getPro());
    if(getIndex() == 1){qury.bindValue(2,ui->comboBox_year->currentText());}
    if(getIndex() == 2){qury.bindValue(2,ui->lineEdit->text());}
    qury.exec();
    qury.first();
    if (getIndex() == 1) {
        if (qury.value(0).toString() == ui->lineEdit_mssv->text() && qury.value(1).toString() == getPro() && qury.value(2).toString() == ui->comboBox_year->currentText()) {
            qDebug()<<"đã đăng kí 1";
            return true;
        }
        else{qDebug()<<"Chưa đăng ksi 1";}
    }
    if (getIndex() == 2) {
        if (qury.value(0).toString() == ui->lineEdit_mssv->text() && qury.value(1).toString() == getPro() && qury.value(2).toString() == ui->lineEdit->text()) {
            qDebug()<<"đã đăng kí 2";
            return true;
        }
        else{qDebug()<<"Chưa đăng ksi 2";}
    }
    qDebug()<<"chưa đăng kí";
    return false;
}

void ChangeMarkOfStudent::setValueForTable(QModelIndex& in,const QString &value){

}


void ChangeMarkOfStudent::on_pushButton_2_clicked()
{
    vector<QString> vv;
    QSqlQuery qury;int x,y;
    QString filter = QString("Excel (*.xlsx ,*.xls)");
    QString filePathName = QFileDialog::getOpenFileName(this,"Select a piture",NULL,filter);
    qury.prepare("INSERT INTO MARK_OF_STUDENTS(MSSV,MAMONHOC,SCHOOL_YEAR,CLASSNAME,kiemTraLan1,kiemTraLan2,kiemTraLan3,DIEMTHUONGXUYEN,DIEMCUOIKI,DIEMTONGKET,THANGDIEM4,DIEMCHU,XEPLOAI)"
                 "VALUES(:mssv,:mamonhoc,:namhoc,:lop,:lan1,:lan2,:lan3,:thuongxuyen,:cuoiki,:tongket,:diem4,:diemchu,:xeploai);");
    QXlsx::Document xlxs(filePathName);

    x = xlxs.dimension().rowCount();y = xlxs.dimension().columnCount();
    qDebug()<<"PTO ->"<<getPro();
    for (int i = 0;i < x ;++i ) {
        if(xlxs.read(i+2,2).toString() == getPro())
        {
            for (int j = 0;j < y ;++j ) {
                vv.push_back(xlxs.read(i+2,j+1).toString());
            }
            qury.bindValue(0,vv[0]);qury.bindValue(1,vv[1]);qury.bindValue(2,vv[2]);qury.bindValue(3,vv[3]);qury.bindValue(4,vv[4]);qury.bindValue(5,vv[5]);qury.bindValue(6,vv[6]);
            qury.bindValue(7,vv[7]);qury.bindValue(8,vv[8]);qury.bindValue(9,vv[9]);qury.bindValue(10,vv[10]);qury.bindValue(11,vv[11]);qury.bindValue(12,vv[12]);
            qury.exec();
            vv.clear();
        }
    }
    emit destroyed();
}

