#include "SubClassForRes.h"

SubClassForRes::SubClassForRes(){}

int SubClassForRes::getIndexOfObject()
{
    return this->indexOfObject;
}

QString SubClassForRes::getCourseCode()
{
    return this->courseCode;
}

QString SubClassForRes::getSubjectName()
{
    return this->subjectName;
}

QDate SubClassForRes::getdateStartStudy()
{
    return this->dateStartStudy;
}

QDate SubClassForRes::getdateEndStudy()
{
    return this->dateEndStudy;
}

void SubClassForRes::getAll(int index_, QString courseCode_, QString subjectName_, QDate dateStart_, QDate dateEnd_)
{
    this->indexOfObject = index_;
    this->courseCode = courseCode_;
    this->subjectName = subjectName_;
    this->dateStartStudy = dateStart_;
    this->dateEndStudy = dateEnd_;
}
