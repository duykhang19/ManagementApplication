#ifndef PERSON_H
#define PERSON_H
#include<QString>
#include<QDebug>
#include<QMessageBox>
#include<QIcon>
#include<QPushButton>
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include"AutoFillPassWord.h"
#include<QCloseEvent>
#include<QColor>

#define Teal QColor(0,128,128)
#define Maroon QColor(128,0,0)
#define Purple QColor(128,0,128)
#define Tomato QColor(255,99,71)
#define Olive QColor(128,128,0)
#define LightSeaGreen QColor(32,178,170)
#define MediumSeaGreen QColor(60,179,113)
#define SteelBlue QColor(70,130,180)
#define SkyBlue QColor(135,206,235)
#define Magenta QColor(255,0,255)
#define HotPink QColor(255,105,180)
#define Chocolate QColor(210,105,30)
#define RosyBrown QColor(188,143,143)
#define Moccasin QColor(112,128,144)
#define SlateGray QColor(139,69,19)
#define DarkViolet QColor(148,0,211)
#define Crimson QColor(220,20,60)
#define Coral QColor(255,127,80)

using namespace std;

class Person
{
private:
    //together
    QString NAME;
    QString USERNAME;
    QString PASSWORD;
    QString ADDRESS;
    QString PHONENUMBER;
    QString SEX;
    QString GMAIL;
    QString IDENTITYCARD;
    QString DATEBIRTH;
    bool checkName;
    bool checkUsername;
    bool checkAddress;
    bool checkDatebirth;
    bool checkPhonenumber;
    bool checkGmail;
    bool checkIdentityCard;
    bool saveOrNot;
public:
    Person();
    vector<AutoFillPassWord> vecT;
    //set
    vector<QColor> vecColor;
    void setName(QString name);//{this->NAME = name;}
    void setUsername(QString username);//{this->USERNAME = username;}
    void setPassWord(QString password);//{this->PASSWORD = password;}
    void setAddress(QString address);//{this->ADDRESS = address;}
    void setPhonenumber(QString phonenumber);//{this->PHONENUMBER = phonenumber;}
    void setSex(QString sex);//{this->SEX = sex;}
    void setGmail(QString gmail);//{this->GMAIL = gmail;}
    void setIdentitycard(QString identitycard);//{this->IDENTITYCARD = identitycard;}
    void setDatebirth(QString datebirth);//{this->DATEBIRTH = datebirth;}
    void setCheckName(bool check);//{this->checkName = check;}
    void setCheckUsername(bool check);//{this->checkUsername = check;}
    void setCheckAddress(bool check);//{this->ADDRESS = check;}
    void setCheckDatebirth(bool check);//{this->checkDatebirth = check;}
    void setCheckPhonenumber(bool check);//{this->checkPhonenumber = check;}
    void setCheckGmail(bool check);//{this->checkGmail = check;}
    void setCheckIdentitycard(bool check);//{this->checkIdentityCard = check;}
    void setSaveOrNot(bool);
    bool getSaveOrNot();
    //get
    QString getName();//{return this->NAME;}
    QString getUsername();//{return this->USERNAME;}
    QString getPassword();//{return this->PASSWORD;}
    QString getAddress();//{return this->ADDRESS;}
    QString getPhonenumber();//{return this->PHONENUMBER;}
    QString getSex();//{return this->SEX;}
    QString getGmail();//{return this->GMAIL;}
    QString getIdentitycard();//{return this->IDENTITYCARD;}
    QString getDatebirth();//{return this->DATEBIRTH;}
    bool getCheckName();//{return this->checkName;}
    bool getCheckUsername();//{return this->checkUsername;}
    bool getCheckAddress();//{return this->checkAddress;}
    bool getCheckDatebirth();//{return this->checkDatebirth;}
    bool getCheckPhonenumber();//{return this->checkPhonenumber;}
    bool getCheckGmail();//{return this->checkGmail;}
    bool getCheckIdentitycard();//{return this->checkIdentityCard;}
    //virtual
    virtual void getInformation();
    virtual bool checkForDuplicateData();
    virtual void doNothing();
    virtual bool checkDataIsNull();
    virtual void insertIntoDatabase();
    virtual void initPasswordHint();
    virtual void passwordShow();
    virtual void passwordHint();
    //messageBox
    void messageBoxOneButton(QString title,QString text, QString textButton, int w, int h,int wb,int hb);
    bool messageBoxTwoButton(QString title,QString text, QString textButton1,QString textButton2, int w, int h,int wb,int hb);
    //write and read file
    void writeToFile();
    void readFile();
    ~Person();
};

#endif // PERSON_H
