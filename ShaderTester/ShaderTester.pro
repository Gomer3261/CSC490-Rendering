#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T23:19:13
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShaderTester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglview.cpp \
    objloader.cpp \
    mesh.cpp \
    scene.cpp \
    orbitallight.cpp \
    renderobject.cpp \
    material.cpp \
    shader.cpp \
    shaderloader.cpp \
    shaderpostprocess.cpp \
    shaderpostprocessripple.cpp \
    shaderpostprocessfocal.cpp \
    objmaterialloader.cpp \
    shadermanager.cpp \
    shadersimplesss.cpp \
    shaderpostprocessglow.cpp \
    shadermultipasssss.cpp \
    shaderpostprocesssssdebug.cpp \
    dragsensitivelistwidget.cpp \
    textureshader.cpp \
    shadermultipassssstextured.cpp

HEADERS  += mainwindow.h \
    openglview.h \
    objloader.h \
    mesh.h \
    scene.h \
    light.h \
    orbitallight.h \
    renderobject.h \
    material.h \
    shader.h \
    shaderloader.h \
    shaderpostprocess.h \
    shaderpostprocessripple.h \
    shaderpostprocessfocal.h \
    objmaterialloader.h \
    shadermanager.h \
    shadersimplesss.h \
    shaderpostprocessglow.h \
    shadermultipasssss.h \
    shaderpostprocesssssdebug.h \
    dragsensitivelistwidget.h \
    textureshader.h \
    shadermultipassssstextured.h

FORMS    += mainwindow.ui
