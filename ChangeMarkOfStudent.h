#ifndef CHANGEMARKOFSTUDENT_H
#define CHANGEMARKOFSTUDENT_H

#include <QWidget>
#include<QToolTip>
#include<QSqlQuery>
#include<QMessageBox>
#include<QString>
#include<iostream>
#include<string>
#include<QDebug>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#include<sstream>
#include<QMap>
#include<QPair>
#include<iterator>
#include"Person.h"
#include<QVector>
#include<QCloseEvent>
#include<QFileDialog>
#include<QFileInfo>
#include<QSqlTableModel>
#include<QTableView>


#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

namespace Ui {
class ChangeMarkOfStudent;
}

class ChangeMarkOfStudent : public QWidget, public Person
{
    Q_OBJECT


public:
    explicit ChangeMarkOfStudent(QWidget *parent = nullptr);
    void Init();
    void setPro(QString);
    QString getPro();
    QString getStudentGrading(double mark);
    bool checkMarkValid(QString);
    double calculateProcessScore(double,double,double);
    double calculateFinalGrade(double,double);
    double calculateFourGradeScale(double);
    void doWhenTextChanged();
    void initMapGrading(QMap<QString,QString>&);
    bool checkIsNotNull();
    void checkTest1A();
    void checkTest2A();
    void checkTest3A();
    void checkFinalPointA();
    bool checkStudentId();
    void editingFinishStuId();
    void solveMark(double,double,double,double);
    void initComboxBox();
    void setSubId(QString);
    QString getSubId();
    void setStudentMark(QString mssv, QString year, QString lan1, QString lan2, QString lan3, QString quatrinh, QString cuoiki, QString tongket, QString thangdiem4, QString diemchu, QString xeploai);
    void clearData();
    void setIndex(int);
    int getIndex();
    void hideLineEdit();
    void showLineEdit();
    void hideComboBox();
    void showComboBox();
    void hideClass();
    void showClass();
    void setTextLineEdit(QString);
    bool checkValidObject();
    bool checkHadRes();
    void setValueForTable(QModelIndex& in,const QString &value);
    ~ChangeMarkOfStudent();
private slots:
    void on_lineEdit_mssv_editingFinished();
    void on_lineEdit_lan1_editingFinished();
    void on_lineEdit_lan2_editingFinished();
    void on_lineEdit_lan3_editingFinished();
    void on_lineEdit_thuongxuyen_editingFinished();
    void on_lineEdit_cuoiki_editingFinished();
    void on_pushButton_capnhat_clicked();
    void on_lineEdit_mssv_textChanged(const QString &arg1);
    void on_lineEdit_lan1_textChanged(const QString &arg1);
    void on_lineEdit_lan2_textChanged(const QString &arg1);
    void on_lineEdit_lan3_textChanged(const QString &arg1);
    void on_lineEdit_cuoiki_textChanged(const QString &arg1);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::ChangeMarkOfStudent *ui;
    QString subId;
    bool check;
    QString pro;
    bool checkTest1;
    bool checkTest2;
    bool checkTest3;
    bool checkEnd;
    bool checkComboxBox;
    QMap<QString,QString> setMap;
    int index;
};

#endif // CHANGEMARKOFSTUDENT_H
