#ifndef SUBCLASSFORRES_H
#define SUBCLASSFORRES_H

#include<QDate>
#include<QString>

class SubClassForRes
{
private:
    int indexOfObject;
    QString courseCode;
    QString subjectName;
    QDate dateStartStudy;
    QDate dateEndStudy;
public:
    SubClassForRes();
    int getIndexOfObject();
    QString getCourseCode();
    QString getSubjectName();
    QDate getdateStartStudy();
    QDate getdateEndStudy();
    void getAll(int,QString,QString,QDate,QDate);
};

#endif // SUBCLASSFORRES_H
