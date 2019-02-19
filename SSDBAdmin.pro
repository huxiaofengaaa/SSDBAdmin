#-------------------------------------------------
#
# Project created by QtCreator 2019-02-19T14:38:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SSDBAdmin
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SSDBCommand.cpp \
    TCPClient.cpp \
    Utility.cpp \
    NoticeDialog.cpp

HEADERS  += MainWindow.h \
    SSDBCommand.h \
    TCPClient.h \
    Utility.h \
    NoticeDialog.h

FORMS    += MainWindow.ui \
    NoticeDialog.ui
