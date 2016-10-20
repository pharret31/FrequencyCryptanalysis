QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

HEADERS += \
    mainwindow.h \
    frequenciestablemodel.h

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    frequenciestablemodel.cpp
