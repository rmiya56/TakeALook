QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow/areaselectionitem.cpp \
    mainwindow/imagehandler.cpp \
    mainwindow/imageproperties.cpp \
    mainwindow/scene.cpp \
    mainwindow/takealookmainwindow.cpp \
    mainwindow/toggleaction.cpp \
    mainwindow/view.cpp \
    baloontip/baloontip.cpp \
    baloontip/pixbaloontip.cpp \
    utility/icons.cpp \
    utility/mouseevent.cpp \
    utility/pathutilities.cpp

HEADERS += \
    mainwindow/areaselectionitem.h \
    mainwindow/imagehandler.h \
    mainwindow/imageproperties.h \
    mainwindow/scene.h \
    mainwindow/takealookmainwindow.h \
    mainwindow/toggleaction.h \
    mainwindow/view.h \
    baloontip/baloontip.h \
    baloontip/pixbaloontip.h \
    utility/icons.h \
    utility/mouseevent.h \
    utility/pathutilities.h

FORMS += \
    mainwindow/takealookmainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:{
include("c:/opencv/opencv.pri")
RC_ICONS = icons/myappico.ico
}
unix:!macx{
CONFIG += link_pkgconfig
PKGCONFIG += opencv
}
unix:macx{
INCLUDEPATH += "/usr/local/include"
LIBS += -L"/usr/local/lib" \
-lopencv_world
}

RESOURCES += \
    resource.qrc

DISTFILES +=
