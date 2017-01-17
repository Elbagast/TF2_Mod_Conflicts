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

SOURCES += main.cpp\
        mainwindow.cpp \
    mod.cpp \
    modconflict.cpp \
    editablelistmodel.cpp \
    editablelistview.cpp \
    filesystem.cpp

HEADERS  += mainwindow.h \
    mod.h \
    modconflict.h \
    less_strcmp_qstring.h \
    editablelistmodel.h \
    editablelistview.h \
    filesystem.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    readme.txt
