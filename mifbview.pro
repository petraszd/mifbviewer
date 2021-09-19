# -------------------------------------------------
# Project created by QtCreator 2009-03-15T12:36:35
# -------------------------------------------------
QT += opengl
TARGET = mifbview
TEMPLATE = app
SOURCES += main.cpp \
    bviewer.cpp \
    datareader.cpp \
    factorial.cpp \
    patch.cpp \
    surface.cpp \
    bezierpatch.cpp \
    mainwindow.cpp \
    boundingbox.cpp \
    polypatch.cpp \
    surfaceoptions.cpp \
    colorselector.cpp \
    curvature.cpp
HEADERS += bviewer.h \
    datareader.h \
    drawable.h \
    factorial.h \
    patch.h \
    drawable.h \
    surface.h \
    bezierpatch.h \
    mainwindow.h \
    boundingbox.h \
    controlpoint.h \
    vertex.h \
    vector3d.h \
    quad.h \
    defaults.h \
    polypatch.h \
    triangle.h \
    surfaceoptions.h \
    colorselector.h \
    curvature.h
FORMS += mainwindow.ui
LIBS += -lCg \
    -lCgGL
