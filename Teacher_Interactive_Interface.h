#ifndef TEACHER_INTERACTIVE_INTERFACE_H
#define TEACHER_INTERACTIVE_INTERFACE_H

#include <QMainWindow>
#include<QDebug>
#include<QSqlQueryModel>
#include<QSqlTableModel>
#include<QSqlQuery>
#include<QString>
#include<QMessageBox>
#include<QCalendarWidget>
#include<QTextCharFormat>
#include<QDate>
#include<QStandardItemModel>
#include<QStandardItem>
#include"ChangeMarkOfStudent.h"
#include"Person.h"
#include<QAbstractItemView>
#include<QModelIndex>
#include<QResizeEvent>
#include<QCheckBox>
#include<QRadioButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSpacerItem>
#include"ChangeInformationOfStudent.h"
#include"ChangePassword.h"
#include<QCloseEvent>
#include<QTimer>
#include<QFileDialog>
#include<QFileInfo>
#include<QtGui>
#include<QComboBox>
#include<QKeyEvent>
#include<QProcess>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;


namespace Ui {
class Teacher_Interactive_Interface;
}

class Teacher_Interactive_Interface : public QMainWindow, public Person
{
    Q_OBJECT

public:
    explicit Teacher_Interactive_Interface(QWidget *parent = nullptr,bool pass = false,QString username="",QString password ="");
    void Init();
    void InitResStu();
    void initTableRes();
    void initTableListCourseOffer();
    void initTableScheduleOffer();
    void setIndex(int);
    int getIndex();
    void clearTimeTable();
    void pullOfData();
    void inittabletime(QDate date);
    void initTableMark();
    void initComboBox();
    void initComboBoxTabDS();
    void deleteRowCurrent();
    void viewMark();
    void updateInfo();
    void setPictureForUser();
    void getSubId();
    void setSubID(QString);
    int getIndexOfTab();
    void setIndexOfTab(int);
    void closeEvent(QCloseEvent* event) override;
    int printMarkOrList();
    void doPrint();
    void doWhenComboBoxChanged(QString arf);
    int doWhenHaveDoubleClickOnTableViewMark();
    void doWhenUiComboBoxYearChange(QString);
    void keyReleaseEvent(QKeyEvent* event) override;

    bool checkCourseIdIsValid();
    bool checkCourseIdIsExist();
    bool checkDateStartIsValid();
    bool checkDateEndIsValid();
    bool checkDateTestIsValid();
    bool checkSemesterIsValid();
    bool checkLimitedQuantityIsValid();
    bool checkDateStudyIsValid();
    bool checkRoomIsValid();
    bool checkAllDateIsValid();
    bool checkDateStuValid();
    bool checkSemesterIsChecked();
    bool checkNumresIsValid();


    void insertCourseIntoDatabase();
    void insertScheduleOfferDatabase();
    void insertIdToCbBoxWhenHaveNewId();
    void whenYearOfCourseChange();
    QString checkDateStudyIsExist();
    ~Teacher_Interactive_Interface();

private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void onSelectionChanged();
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_11_clicked();
    void on_pushButton_10_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButton_9_clicked();
    void on_pushButton_13_clicked();
    void tickChange(int index);
    void on_comboBox_op_currentIndexChanged(int index);
    void on_radioButton_TC_clicked();
    void on_radioButton_CT_clicked();
    void on_comboBox_year_currentIndexChanged(int index);
    void on_comboBox_class_currentIndexChanged(int index);
    void showText();
    void on_pushButton_14_clicked();
    void on_actionTh_ng_tin_c_c_nh_n_triggered();
    void on_actionC_p_nh_t_th_ng_tin_c_nh_n_triggered();
    void on_actionQu_n_l_sinh_vi_n_triggered();
    void on_actionL_ch_gi_ng_d_y_triggered();
    void on_actionTh_m_i_m_cho_sinh_vi_n_triggered();
    void on_actionX_a_i_m_sinh_vi_n_triggered();
    void on_actionCh_nh_s_a_i_m_triggered();
    void on_action_i_m_t_kh_u_triggered();
    void on_actionTho_t_triggered();
    void on_actionDanh_s_ch_sinh_vi_n_triggered();
    void on_actionL_ch_thi_triggered();
    void setTime();
    void on_action_ng_xu_t_triggered();
    void on_pushButton_12_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_15_clicked();
    void on_calendarWidget_clicked(const QDate &date);
    void on_lineEdit_courseId_textChanged(const QString &arg1);
    void on_lineEdit_dateStart_textChanged(const QString &arg1);
    void on_lineEdit_dateEnd_textChanged(const QString &arg1);
    void on_lineEdit_dateTest_textChanged(const QString &arg1);
    void on_lineEdit_semester_textChanged(const QString &arg1);
    void on_lineEdit_quantity_textChanged(const QString &arg1);
    void on_pushButton_createCourse_clicked();
    void on_lineEdit_dateStu_textChanged(const QString &arg1);
    void on_lineEdit_class_textChanged(const QString &arg1);
    void on_pushButton_createDay_clicked();
    void on_tableView_listCourse_doubleClicked(const QModelIndex &index);
    void on_tableView_ScheduleCourse_doubleClicked(const QModelIndex &index);
    void on_action_xu_t_h_c_ph_n_triggered();
    void on_actionKh_i_ng_l_i_triggered();

    void on_lineEdit_numRes_textChanged(const QString &arg1);

private:
    Ui::Teacher_Interactive_Interface *ui;
    QCalendarWidget* calendar;QString msgv;
    int index,indexx;
    QSqlTableModel* tableModel,*tableListOffer,*tableScheduleOffer;
    QSqlQueryModel* quryModel;
    QStandardItemModel* itemModel;
    ChangeMarkOfStudent* changeMark;
    ChangeInformationOfStudent* changeInfo;
    QString professional;
    ChangePassword* changePW;
    int a,indexOfTab,inPrint,xRe;
    bool checkClose;
    QTimer* timer;
    QWidget* widget;
    QHBoxLayout* hLayout; QLabel* labelYear;QLabel* labelClass;QPushButton* conti;
    QComboBox* comboxYear;QComboBox* comboxClass;QVBoxLayout* vLayout;
    QSpacerItem* itemSpacer;
};

#endif // TEACHER_INTERACTIVE_INTERFACE_H
