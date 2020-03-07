QT = core gui widgets charts

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += main.cpp \
    chartperstory.cpp \
    donutbreakdownchart.cpp \
    flowlayout.cpp \
    mainslice.cpp \
    mainwindow.cpp \
    stripsgrid.cpp \
    stripwidget.cpp

FORMS += \
    mainwindow.ui \
    stripwidget.ui

HEADERS += \
    chartperstory.h \
    donutbreakdownchart.h \
    flowlayout.h \
    mainslice.h \
    mainwindow.h \
    stripsgrid.h \
    stripwidget.h

OTHER_FILES += README.md \
    .travis.yml
