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
#include<QCloseEvent>
#include<QColor>

//#define Teal QColor(0,128,128)
//#define Maroon QColor(128,0,0)
//#define Purple QColor(128,0,128)
//#define Tomato QColor(255,99,71)
//#define Olive QColor(128,128,0)
//#define LightSeaGreen QColor(32,178,170)
//#define MediumSeaGreen QColor(60,179,113)
//#define SteelBlue QColor(70,130,180)
//#define SkyBlue QColor(135,206,235)
//#define Magenta QColor(255,0,255)
//#define HotPink QColor(255,105,180)
//#define Chocolate QColor(210,105,30)
//#define RosyBrown QColor(188,143,143)
//#define Moccasin QColor(112,128,144)
//#define SlateGray QColor(139,69,19)
//#define DarkViolet QColor(148,0,211)
//#define Crimson QColor(220,20,60)
//#define Coral QColor(255,127,80)

using namespace std;

class Person
{
public:
    Person();
    //messageBox
    void messageBoxOneButton(QString title,QString text, QString textButton, int w, int h,int wb,int hb);
    bool messageBoxTwoButton(QString title,QString text, QString textButton1,QString textButton2, int w, int h,int wb,int hb);
    ~Person();
};

#endif // PERSON_H
