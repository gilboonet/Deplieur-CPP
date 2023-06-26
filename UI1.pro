QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    const.cpp \
    copl.cpp \
    edge.cpp \
    facette.cpp \
    flap.cpp \
    lineitem.cpp \
    main.cpp \
    mainwindow.cpp \
    neighbor.cpp \
    numitem.cpp \
    page.cpp \
    piece.cpp \
    titleitem.cpp \
    triangle2d.cpp \
    triangle3d.cpp \
    unfold.cpp

HEADERS += \
    const.h \
    copl.h \
    edge.h \
    facette.h \
    flap.h \
    lineitem.h \
    mainwindow.h \
    neighbor.h \
    numitem.h \
    page.h \
    piece.h \
    titleitem.h \
    triangle2d.h \
    triangle3d.h \
    unfold.h

FORMS += \
    mainwindow.ui

#TRANSLATIONS =  UI1_fr.ts \
#                UI1_en.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
