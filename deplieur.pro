TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        const.cpp \
        copl.cpp \
        edge.cpp \
        facette.cpp \
        main.cpp \
        neighbor.cpp \
        page.cpp \
        piece.cpp \
        triangle2d.cpp \
        triangle3d.cpp \
        unfold.cpp \
        vec2.cpp \
        vec3.cpp

HEADERS += \
    const.h \
    copl.h \
    edge.h \
    facette.h \
    neighbor.h \
    page.h \
    piece.h \
    triangle2d.h \
    triangle3d.h \
    unfold.h \
    vec2.h \
    vec3.h
