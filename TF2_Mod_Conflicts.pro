#-------------------------------------------------
#
# Project created by QtCreator 2014-10-29T18:49:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TF2_Mod_Conflicts
TEMPLATE = app

# Remember to use 32bit compiler...
CONFIG += static
CONFIG += threads
win32:LIBS += -L"C:/Boost/boost_1_55_0/stage/lib"
win32:INCLUDEPATH += "C:/Boost/boost_1_55_0"
win32:DEPENDPATH += "C:/Boost/boost_1_55_0/stage/lib"

SOURCES += main.cpp\
        mainwindow.cpp \
    mod.cpp \
    modconflict.cpp \
    editablelistmodel.cpp \
    editablelistview.cpp

HEADERS  += mainwindow.h \
    mod.h \
    qt_boost/filesystem.h \
    modconflict.h \
    less_strcmp_qstring.h \
    editablelistmodel.h \
    editablelistview.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    readme.txt
