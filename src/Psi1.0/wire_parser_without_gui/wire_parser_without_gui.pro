#-------------------------------------------------
#
# Project created by QtCreator 2014-10-14T22:17:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Psi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    opening_window.cpp \
    interconnection_network.cpp \
    treeitem.cpp \
    tree_item_module.cpp \
    ident.cpp \
    network.cpp \
    option.cpp \
    inputform.cpp \
    assertion_interface.cpp

HEADERS  += mainwindow.h \
    opening_window.h \
    interconnection_network.h \
    treeitem.h \
    tree_item_module.h \
    ident.h \
    network.h \
    option.h \
    inputform.h \
    Abs_network.h \
    assert_type.h \
    assertion_interface.h \
    assert_interface.h

FORMS    +=

OTHER_FILES += \
