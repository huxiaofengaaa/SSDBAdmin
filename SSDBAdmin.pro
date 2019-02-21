#-------------------------------------------------
#
# Project created by QtCreator 2019-02-19T14:38:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SSDBAdmin
TEMPLATE = app

INCLUDEPATH += .\asio-1.12.2\include
DEFINES += ASIO_STANDALONE
LIBS += -lws2_32

SOURCES += main.cpp\
        MainWindow.cpp \
    SSDBCommand.cpp \
    TCPClient.cpp \
    Utility.cpp \
    NoticeDialog.cpp \
    SSDBHandler.cpp

HEADERS  += MainWindow.h \
    SSDBCommand.h \
    TCPClient.h \
    Utility.h \
    NoticeDialog.h \
    SSDBHandler.h

FORMS    += MainWindow.ui \
    NoticeDialog.ui
