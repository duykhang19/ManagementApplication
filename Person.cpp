#include "Person.h"


Person::Person()
{
    this->saveOrNot = false;
    vecColor.push_back(Teal);
    vecColor.push_back(Maroon);
    vecColor.push_back(Purple);
    vecColor.push_back(Tomato);
    vecColor.push_back(Olive);
    vecColor.push_back(LightSeaGreen);
    vecColor.push_back(MediumSeaGreen);
    vecColor.push_back(SteelBlue);
    vecColor.push_back(SkyBlue);
    vecColor.push_back(Magenta);
    vecColor.push_back(HotPink);
    vecColor.push_back(Chocolate);
    vecColor.push_back(RosyBrown);
    vecColor.push_back(Moccasin);
    vecColor.push_back(SlateGray);
    vecColor.push_back(DarkViolet);
    vecColor.push_back(Crimson);
    vecColor.push_back(Coral);
}
void Person::setName(QString name){this->NAME = name;}
void Person::setUsername(QString username){this->USERNAME = username;}
void Person::setPassWord(QString password){this->PASSWORD = password;}
void Person::setAddress(QString address){this->ADDRESS = address;}
void Person::setPhonenumber(QString phonenumber){this->PHONENUMBER = phonenumber;}
void Person::setSex(QString sex){this->SEX = sex;}
void Person::setGmail(QString gmail){this->GMAIL = gmail;}
void Person::setIdentitycard(QString identitycard){this->IDENTITYCARD = identitycard;}
void Person::setDatebirth(QString datebirth){this->DATEBIRTH = datebirth;}
void Person::setCheckName(bool check){this->checkName = check;}
void Person::setCheckUsername(bool check){this->checkUsername = check;}
void Person::setCheckAddress(bool check){this->checkAddress = check;}
void Person::setCheckDatebirth(bool check){this->checkDatebirth = check;}
void Person::setCheckPhonenumber(bool check){this->checkPhonenumber = check;}
void Person::setCheckGmail(bool check){this->checkGmail = check;}
void Person::setCheckIdentitycard(bool check){this->checkIdentityCard = check;}
//get
QString Person::getName(){return this->NAME;}
QString Person::getUsername(){return this->USERNAME;}
QString Person::getPassword(){return this->PASSWORD;}
QString Person::getAddress(){return this->ADDRESS;}
QString Person::getPhonenumber(){return this->PHONENUMBER;}
QString Person::getSex(){return this->SEX;}
QString Person::getGmail(){return this->GMAIL;}
QString Person::getIdentitycard(){return this->IDENTITYCARD;}
QString Person::getDatebirth(){return this->DATEBIRTH;}
void Person::setSaveOrNot(bool a){this->saveOrNot = a;}
bool Person::getSaveOrNot(){return this->saveOrNot;}
bool Person::getCheckName(){return this->checkName;}
bool Person::getCheckUsername(){return this->checkUsername;}
bool Person::getCheckAddress(){return this->checkAddress;}
bool Person::getCheckDatebirth(){return this->checkDatebirth;}
bool Person::getCheckPhonenumber(){return this->checkPhonenumber;}
bool Person::getCheckGmail(){return this->checkGmail;}
bool Person::getCheckIdentitycard(){return this->checkIdentityCard;}
//virtual
void Person::getInformation(){}
bool Person::checkForDuplicateData(){return true;}
void Person::doNothing(){}
bool Person::checkDataIsNull(){return true;}
void Person::insertIntoDatabase(){}
void Person::initPasswordHint(){}
void Person::passwordShow(){}
void Person::passwordHint(){}
//messageBox
void Person:: messageBoxOneButton(QString title,QString text, QString textButton, int w, int h,int wb,int hb)
{
    QMessageBox mess;
    mess.setFixedSize(w,h);
    mess.setText(text);
    mess.setWindowIcon(QIcon(":/icon-file/waning.png"));
    mess.setWindowTitle(title);
    mess.setStyleSheet("font:8pt Courier;background-color:white;padding-bottom:5px;");

    QPushButton* buttonYes = mess.addButton(textButton,QMessageBox::YesRole);
    buttonYes->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline:0}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}"
    "QPushButton:pressed{background-color: rgb(255, 170, 0);border:1px solid rgb(68, 68, 68);outline : 0;");
    buttonYes->setFocusPolicy(Qt::NoFocus);
    buttonYes->setFixedSize(wb,hb);

    mess.exec();
    if (mess.clickedButton() == buttonYes){mess.close();}
}

bool Person::messageBoxTwoButton(QString title,QString text, QString textButton1,QString textButton2, int w, int h,int wb,int hb)
{
    QMessageBox mes;
    mes.setWindowTitle(title);
    mes.setText(text);
    mes.setWindowIcon(QIcon(":/icon-file/waning.png"));
    mes.setFixedSize(w,h);
    mes.setStyleSheet("font : 8pt Courier");
    QPushButton* buttonYes = mes.addButton(textButton1,QMessageBox::YesRole);
    QPushButton* buttonNo = mes.addButton(textButton2,QMessageBox::NoRole);
    buttonYes->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
    buttonNo->setStyleSheet("QPushButton{background-color:white;border : 2px groove rgb(255, 170, 0);border-radius : 10px;height : 20;width : 120;}"
    "QPushButton:hover{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);}"
    "QPushButton:focus{background-color: rgb(255, 170, 0);border:2px groove rgb(68, 68, 68);outline : 0;}");
    buttonYes->setFixedSize(wb,hb);
    buttonNo->setFixedSize(wb,hb);
    buttonYes->setFocusPolicy(Qt::NoFocus);
    buttonNo->setFocusPolicy(Qt::NoFocus);
    mes.exec();
    if(mes.clickedButton() == buttonYes){mes.close();return true;}
    return false;
}

void Person::writeToFile()
{
    fstream fileWrite;
    if(fileWrite.good() && getSaveOrNot())
    {
        fileWrite.open("C:\\Users\\duy khan\\Documents\\projectQt5\\data.txt",ios::out | ios::trunc);
        for (auto i = vecT.begin(); i != vecT.end() ; i++) {
            fileWrite<<"true"<<"\n"<<i->getUser().toStdString()<<"\n"<<i->getPass().toStdString()<<"\n";
            qDebug()<<"WRITE FILE :: "<<i->getUser()<<":"<<i->getPass();
        }
    }
    fileWrite.close();
}


void Person::readFile()
{
    AutoFillPassWord au;
    fstream readFileF;
    string f;
    readFileF.open("C:\\Users\\duy khan\\Documents\\projectQt5\\data.txt",ios::in);
    while(!readFileF.eof())
    {
        getline(readFileF,f,'\n');
        if(!f.empty())
        {
            if(f=="true"){au.setBool(true);}
            if(f=="false"){au.setBool(false);}
        }
        getline(readFileF,f,'\n');
        if(!f.empty()){au.setUser(f.c_str());}
        getline(readFileF,f,'\n');
        if(!f.empty()){au.setPass(f.c_str());vecT.push_back(au);}
    }
    for(unsigned long long i = 0;i < vecT.size();i++)
    {
        qDebug()<<vecT[i].getBool()<<":"<<vecT[i].getUser()<<":"<<vecT[i].getPass()<<"\n";
    }
    readFileF.close();
}

//destructor
Person::~Person(){}
