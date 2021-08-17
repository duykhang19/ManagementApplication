#ifndef AUTOFILLPASSWORD_H
#define AUTOFILLPASSWORD_H

#include<QString>
#include<QDebug>

class AutoFillPassWord
{
private:
    bool trueOrFalse;
    QString user,pass;
public:
    //set
    void setBool(bool);
    void setUser(QString);
    void setPass(QString);
    //get
    bool getBool();
    QString getUser();
    QString getPass();
    AutoFillPassWord(bool=false,QString="",QString="");
};

#endif // AUTOFILLPASSWORD_H
