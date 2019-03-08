#-------------------------------------------------
#
# Project created by QtCreator 2018-10-29T13:22:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core gui multimedia multimediawidgets

TARGET = bitirmevideogui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        mygraphicsview.cpp \
        reciever.cpp \
        decryption.cpp \
        encryption.cpp \
        keygenerator.cpp \
        tables.cpp \
        decryption.h \
        encryption.h \
        keygenerator.h \
        tables.h \
    canny.cpp \
    tracking.cpp \
    testscreen.cpp

HEADERS += \
        mainwindow.h \
    mygraphicsview.h \
    reciever.h \
    canny.h \
    tracking.h \
    testscreen.h

FORMS += \
        mainwindow.ui \
    testscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
