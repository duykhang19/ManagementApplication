QT       += core widgets gui sql network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#//this
# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./source/  # current QXlsx source path is ./source/
include(./QXlsx.pri)
#this

SOURCES += \
    AutoFillPassWord.cpp \
    ChangeInformationOfStudent.cpp \
    ChangeMarkOfStudent.cpp \
    ChangePassword.cpp \
    Lecturer_LogIn_Form.cpp \
    Person.cpp \
    Student_Interactive_Interface.cpp \
    Student_LogIn_Form.cpp \
    SubClassForRes.cpp \
    TeacherOrStudent.cpp \
    Teacher_Interactive_Interface.cpp \
    connectToSql.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AutoFillPassWord.h \
    ChangeInformationOfStudent.h \
    ChangeMarkOfStudent.h \
    ChangePassword.h \
    Lecturer_LogIn_Form.h \
    Person.h \
    Student_Interactive_Interface.h \
    Student_LogIn_Form.h \
    SubClassForRes.h \
    TeacherOrStudent.h \
    Teacher_Interactive_Interface.h \
    connectToSqlServer.h \
    mainwindow.h

FORMS += \
    ChangeInformationOfStudent.ui \
    ChangeMarkOfStudent.ui \
    ChangePassword.ui \
    Lecturer_LogIn_Form.ui \
    Student_Interactive_Interface.ui \
    Student_LogIn_Form.ui \
    TeacherOrStudent.ui \
    Teacher_Interactive_Interface.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

SUBDIRS += \
    QXlsx.pro

DISTFILES += \
    QXlsx.pri
