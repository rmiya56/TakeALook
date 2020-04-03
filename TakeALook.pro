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
    baloontip/baloontip.cpp \
    baloontip/pixbaloontip.cpp \
    overlaypixmap/addpolygoncommand.cpp \
    overlaypixmap/contouritem.cpp \
    overlaypixmap/cv_contours.cpp \
    overlaypixmap/jsonfile.cpp \
    overlaypixmap/overlaypixmapitem.cpp \
    overlaypixmap/overlaypixmaptoolbar.cpp \
    src/areaselectionitem.cpp \
    src/icons.cpp \
    src/imagehandler.cpp \
    src/imageproperties.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/scene.cpp \
    src/toggleaction.cpp \
    src/view.cpp \
    utility/pathutilities.cpp

HEADERS += \
    baloontip/baloontip.h \
    baloontip/pixbaloontip.h \
    overlaypixmap/addpolygoncommand.h \
    overlaypixmap/contouritem.h \
    overlaypixmap/cv_contours.h \
    overlaypixmap/jsonfile.h \
    overlaypixmap/overlaypixmapitem.h \
    overlaypixmap/overlaypixmaptoolbar.h \
    src/areaselectionitem.h \
    src/icons.h \
    src/imagehandler.h \
    src/imageproperties.h \
    src/mainwindow.h \
    src/scene.h \
    src/toggleaction.h \
    src/view.h \
    utility/pathutilities.h


FORMS += \
    src/mainwindow.ui

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
