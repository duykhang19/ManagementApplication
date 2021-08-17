#include "AutoFillPassWord.h"


AutoFillPassWord::AutoFillPassWord(bool a, QString b, QString c)
{
    this->trueOrFalse = a;
    this->user = b;
    this->pass = c;
}

void AutoFillPassWord::setBool(bool a){this->trueOrFalse = a;}
void AutoFillPassWord::setUser(QString a){this->user = a;}
void AutoFillPassWord::setPass(QString a){this->pass = a;}

bool AutoFillPassWord::getBool(){return this->trueOrFalse;}
QString AutoFillPassWord::getUser(){return this->user;}
QString AutoFillPassWord::getPass(){return this->pass;}

