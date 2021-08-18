#ifndef STUDENT_INTERACTIVE_INTERFACE_H
#define STUDENT_INTERACTIVE_INTERFACE_H

#include <QMainWindow>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QSqlTableModel>
#include<QGraphicsScene>
#include<QLabel>
#include<QWidget>
#include<QDropEvent>
#include<QMimeData>
#include<QFileInfo>
#include<QModelIndexList>
#include<QModelIndex>
#include<QKeyEvent>
#include"ChangeInformationOfStudent.h"
#include<QApplication>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QCalendarWidget>
#include<QMouseEvent>
#include<QComboBox>
#include<QCheckBox>
#include<QVector>
#include<QTableView>
#include<QResource>
#include<QByteArray>
#include<iostream>
#include<string>
#include<QFileDialog>
#include<QtWidgets>
#include<QHeaderView>
#include<QHoverEvent>
#include<QTimer>
#include<vector>
#include<iostream>
#include<QMap>
#include<QVector>
#include<windows.h>
#include<QtCharts>
#include<QLineSeries>
#include<QChartView>
#include<QPointF>
#include<QBarSet>
#include<QBarSeries>
#include<QStringList>
#include<QBarCategoryAxis>
#include<QValueAxis>
#include<QGraphicsRectItem>
#include<QPen>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QColor>
#include<QKeyEvent>
#include<QMap>
#include<qiterator.h>
#include<QMenu>
#include<QAction>
#include<QSpacerItem>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

using namespace QXlsx;

using namespace std;

namespace Ui {
class Student_Interactive_Interface;
}

class Student_Interactive_Interface : public QMainWindow, public Person
{
    Q_OBJECT

public:
    explicit Student_Interactive_Interface(QWidget *parent = nullptr);
    //void getIndex(int);
    void getIndexTableViewResA(int);
    void getIndexTableViewResB(int);
    void detailCourse();
    QWidget* wid,*widDebtOfStudent;
    QSqlTableModel* tableModelDebt;
    QTableView* tableVieww,*tbViewStudent,*tbViewTeacher,*tbViewDebtOfStudent,*tbViewMarkStudent,*tbViewRes;
    void closeEvent(QCloseEvent* event) override;
    void returnCheck();
    void insertToRes(int);
    ~Student_Interactive_Interface();
private:
    QString myStuId;int numR,dbc,tbA,tbB,sl;
    Ui::Student_Interactive_Interface *ui;
    QSqlQueryModel *qrymodall;
    ChangeInformationOfStudent *updateInformation;
    int index,indexForTableResA,indexForTableResB,k,mn,nm;
    QSqlQueryModel* tableModel,*tb;
    QSqlTableModel* tableModelRes,*tableModelCanCRes,*detailSchedule,*tbStudent,*tbTeacher,*tbTimeRes,*tbDebtOfStudent;
    QSqlTableModel* tbCoursePassed,*tbDebt,*tbManageMarkStudent,*tbRes;
    QString text;
    QTimer* timer;bool checkTime;QString ti;
    bool checkClose,l,m,nn,ndebt,ydebt,nj,mj;
    QWidget* widget,*widAddNewTeacher,*widMarkStudent,*widRes;
    QMap<QString,QString> mapp;
    int indexTabWidget,indexOptions;
    QChartView* chartView;
    QBarSeries* series;QChart* chart;QBarCategoryAxis* axis;QValueAxis* axisY;QBarCategoryAxis *axisX;
    QBarSet *set0;QBarSet *set1;QBarSet *set2;QBarSet *set3;QBarSet *set4;QBarSet *set5;QBarSet *set6;QBarSet *set7;QBarSet *set8;QBarSet *set9;QBarSet *set10;
    vector<QBarSet*> vecBar;
    QComboBox* comboxYear,*comboxClass,*comboxSemester,*comSemester,*comSub;QLabel* labelYear,*labelId,*labelName,*labelMajor,*labelCLass,*labelSemester,*labelClas,*lbSub,*lbSemester;
    QPushButton* conti,*createPerson,*viewButton,*viewRes;
    QLineEdit* lineEditId,*lineEditName,*lineEditMajor,*lineEditClass;
    QHBoxLayout* hLayout,*hhLayout,*hhLayout1,*hhhLayout,*hhhhLayout,*hhhhhLayout;QVBoxLayout* vLayout,*vvLayout,*vvLayout1,*vvLayout2,*vvvLayout,*vvvvLayout,*vvvvvLayout;
    int a;
private slots:
    void on_pushButton_xem_clicked();
    void on_tabWidget_tabCloseRequested(int index);
    void Init();
    void initTableManageTeacher();
    void initTableManageStudent();
    void initTableManageCourse();
    void initTableManageCoursePassed();
    void initTableCoursePassed();
    void initTableDebt();
    void initTableTimeRes();
    void initTableDebtOfStudent();
    void initSubDebtTable();
    void initTableManageMarkStudent();
    void initAllTableMark();
    void initTableRes();
    void initSubRes();
    void initComboBox();
    void viewResClicked();
    void resForAll();
    void doubleClickOnTbView(int);
    void initComboBoxForMC();
    void initOptionsComboBox();
    void initComboBoxDebt();
    void initComboBoxCourse();
    void initComboBoxDebtOfStudent();
    void initComboBoxMark();
    void initComboRes();
    void initWidAddNewPerson();
    bool checkIDIsExisted();
    bool checkNewTeacherIsValid();
    void initTableStudent();
    void initTableTeacher();
    void ifComboBoxNull(int);
    void comboboxDebtChange();
    void initComboBoxTimeRes();
    void refreshMarkTable();
    void viewButtonClicked();
    void tableDebtDoubleClick(QModelIndex in);
    void clearSelection();
    bool checkIdDebtValid();
    bool checkDebtValid();
    bool checkSemesterValid();
    bool checkYValid();
    void timeResChange();
    void on_comboBox_options_activated(int index); // combobox options
    void on_tableView_registerCourse_clicked(const QModelIndex &index);
    void on_pushButton_cancelRegis_clicked();
    void on_pushButton_regis_clicked();
    void on_tableView_registeredCourse_clicked(const QModelIndex &index);
    void UpdateTime();
    void showRowChange();
    void showWidChangeInfor();
    void showUpConTextMenu(const QPoint &pos);
    void showUpConTextMenu1(const QPoint &pos);
    void showUpConTextMenu2(const QPoint &pos);
    void showUpConTextMenu3(const QPoint &pos);
    void showUpConTextMenu4(const QPoint &pos);
    void showUpConTextMenu6(const QPoint &pos);
    void refreshDebtRes();
    void doubleClickedOnTbStudent(const QModelIndex &index);
    void doubleClickedOnTbTeacher(const QModelIndex &index);
    void textLineEditMajorChange();
    void textLineEditIDChange();
    void textLineEditNameChange();
    void refreshListCourse();
    void printListTeacherAndStudent();
    void prinListDebtMarkRes();
    void mChange();
    void delTeacher(int);
    void addTeacher();
    void editTeacher(int);
    void freshTable();
    void idTrue();
    void doFreshTbListCourse();
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_teacher_clicked();
    void on_pushButton_student_clicked();
    void on_tableView_registerCourse_doubleClicked(const QModelIndex &index);
    void on_tableView_registeredCourse_doubleClicked(const QModelIndex &index);
    void on_tableView_teacher_clicked(const QModelIndex &index);
    void on_tableView_student_clicked(const QModelIndex &index);
    void on_pushButton_updateInfor_clicked();
    void on_lineEdit_id_textChanged(const QString &arg1);
    void on_lineEdit_debt_textChanged(const QString &arg1);
    void on_lineEdit_semester_textChanged(const QString &arg1);
    void on_pushButton_createDebt_clicked();
    void on_tableView_listDebt_doubleClicked(const QModelIndex &index);
    void on_comboBox_debt_currentIndexChanged(const QString &arg1);
    void on_comboBox_course_currentIndexChanged(const QString &arg1);
    void on_tableView_openCourse_clicked(const QModelIndex &index);
    void on_tableView_listDebt_clicked(const QModelIndex &index);
    void on_lineEdit_year_textChanged(const QString &arg1);
    void on_lineEdit_timeRes_textChanged(const QString &arg1);
    void on_pushButton_clicked();
    void on_tableView_timeRes_doubleClicked(const QModelIndex &index);
    void on_tableView_timeRes_clicked(const QModelIndex &index);
    void on_actionTh_ng_tin_sinh_vi_n_triggered();
    void on_actionK_t_qua_ho_c_t_p_triggered();
    void on_actionLi_ch_ho_c_triggered();
    void on_actionLi_ch_thi_triggered();
    void on_actionThoa_t_triggered();
    void on_action_i_m_t_kh_u_triggered();
    void on_actionKh_i_ng_l_i_triggered();
    void on_actionThoa_t_2_triggered();
};


#endif // STUDENT_INTERACTIVE_INTERFACE_H
