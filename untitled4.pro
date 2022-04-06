QT = gui
QT += network
QT += xml
QT       += opengl

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Barrier.cpp \
        Car.cpp \
        GameMap.cpp \
        Visualizer.cpp \
        XMLGenerator.cpp \
        main.cpp \
        mainwindow.cpp \
        mytcpserver.cpp \
        mytcpsocket.cpp \
        point.cpp \
        settings.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Barrier.h \
    Car.h \
    GameMap.h \
    Visualizer.h \
    XMLGenerator.h \
    mainwindow.h \
    mytcpserver.h \
    mytcpsocket.h \
    point.h \
    settings.h

LIBS += -lOpenGL -lGLU

target.path = $$[QT_INSTALL_EXAMPLES]/opengl/2dpainting

FORMS += \
    mainwindow.ui

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
