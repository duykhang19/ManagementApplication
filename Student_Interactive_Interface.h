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
//#include"EditTableSqlQuery.h"
#include<QKeyEvent>
#include"ChangeInformationOfStudent.h"
#include<QApplication>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QCalendarWidget>
#include<QMouseEvent>
#include<QComboBox>
#include<QCheckBox>
#include<QSqlTableModel>
#include"SubClassForRes.h"
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
#include"Person.h"
#include<QMap>
#include<QVector>
#include"ChangePassword.h"
#include<windows.h>
#include"AutoFillPassWord.h"
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
    explicit Student_Interactive_Interface(QWidget *parent = nullptr,bool pass = false,QString username = NULL, QString password = NULL);
    void REFRESH();
    void initTimeTable();
    void inittabletime(QDate date);
    void getIndex(int);
    void getIndexTableViewResA(int);
    void getIndexTableViewResB(int);
    bool checkDuplicateObject(QString);
    int returnIndex(int x);
    bool checkTrueExistForEveryRow();
    void getText(QString txt);
    void initAgain(QString);
    QString getString(QString stringg);
    SubClassForRes qOObject;
    QVector<SubClassForRes> vec;
    void updateDebtStudent();
    QWidget* wid;
    QSqlTableModel* tableModelDebt;
    QTableView* tableVieww;
    void setPictureForUser();
    void semesterChange(QString);
    void initComboBox();
    bool overRegister();
    void execQuery();
    void closeEvent(QCloseEvent* event) override;
    void initGraph();
    void setStuId(QString);
    int doPrintSchedule();
    void printSchedule();
    void printTestSchedule();
    void printMark();
    void createWidgetForOptionPrints();
    void keyReleaseEvent(QKeyEvent* event) override;
    void initQMap();
    QString getStuId();
    ~Student_Interactive_Interface();
private:
    QString myStuId;
    Ui::Student_Interactive_Interface *ui;
    QSqlQueryModel *qrymodall;
    QGraphicsScene *display;
    ChangeInformationOfStudent *updateInformation;
    QCalendarWidget* calendar;
    int index,indexForTableResA,indexForTableResB;
    QSqlTableModel* tableModel;
    QSqlTableModel* tableModelRes;
    QString text;
    QTimer* timer;
    ChangePassword* passWordChange;
    bool checkClose;
    QWidget* widget;
    QMap<QString,QString> mapp;

    QChartView* chartView;
    QBarSeries* series;QChart* chart;QBarCategoryAxis* axis;QValueAxis* axisY;QBarCategoryAxis *axisX;
    QBarSet *set0;QBarSet *set1;QBarSet *set2;QBarSet *set3;QBarSet *set4;QBarSet *set5;QBarSet *set6;QBarSet *set7;QBarSet *set8;QBarSet *set9;QBarSet *set10;
    vector<QBarSet*> vecBar;
    QComboBox* comboxYear;QLabel* labelYear;QPushButton* conti;
    QHBoxLayout* hLayout;QVBoxLayout* vLayout;
    int a;
private slots:
    void on_pushButton_xem_clicked();
    void on_tabWidget_tabCloseRequested(int index);
    void on_pushButton_information_clicked();
    void on_pushButton_mark_clicked();
    void tabSelected();
    void Init();
    void showComboBoxUpYear();
    void showComboBoxUpSubject();
    bool checkUpdateFormIsVisible();
    void on_pushButton_updateInfor_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void pullOfData();
    void clearTimeTable();
    void on_comboBox_options_activated(int index);
    void onSelectionChanged();
    void on_pushButton_4_clicked();
    void on_tableView_registerCourse_clicked(const QModelIndex &index);
    void on_pushButton_cancelRegis_clicked();
    void on_pushButton_regis_clicked();
    void on_tableView_registeredCourse_clicked(const QModelIndex &index);
    void on_actionTh_ng_tin_sinh_vi_n_triggered();
    void on_actionK_t_qua_ho_c_t_p_triggered();
    void on_actionLi_ch_ho_c_triggered();
    void on_actionLi_ch_thi_triggered();
    void on_action_ng_ki_ho_c_ph_n_triggered();
    void on_actionThoa_t_triggered();
    void on_actionThoa_t_2_triggered();
    void UpdateTime();
    void on_action_ng_xu_t_triggered();
    void showRowChange();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_action_i_m_t_kh_u_triggered();
    void displayMarkOfGraph(bool ch, int cl,QBarSet* bar);
    void on_comboBox_subject_currentIndexChanged(int index);
    void on_comboBox_yearSch_currentIndexChanged(int index);
    void on_tabWidget_currentChanged(int index);
    void on_actionIn_triggered();
    void on_pushButton_5_clicked();
    void on_actionKh_i_ng_l_i_triggered();
};

#endif // STUDENT_INTERACTIVE_INTERFACE_H
