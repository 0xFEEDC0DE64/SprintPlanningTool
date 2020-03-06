QT = core gui widgets

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += main.cpp \
    flowlayout.cpp \
    mainwindow.cpp \
    stripsgrid.cpp \
    stripwidget.cpp

FORMS += \
    mainwindow.ui \
    stripwidget.ui

HEADERS += \
    flowlayout.h \
    mainwindow.h \
    stripsgrid.h \
    stripwidget.h

OTHER_FILES += README.md \
    .travis.yml
