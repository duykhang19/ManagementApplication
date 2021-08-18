QT       += core gui sql charts network

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
    ChangeInformationOfStudent.cpp \
    Person.cpp \
    Student_Interactive_Interface.cpp \
    connectToSql.cpp \
    main.cpp

HEADERS += \
    ChangeInformationOfStudent.h \
    Person.h \
    Student_Interactive_Interface.h \
    connectToSqlServer.h

FORMS += \
    ChangeInformationOfStudent.ui \
    Student_Interactive_Interface.ui

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
