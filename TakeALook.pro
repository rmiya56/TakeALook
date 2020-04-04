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
    takealook/mainwindow.cpp \
    takealook/view.cpp \
    takealook/scene.cpp \
    takealook/areaselectionitem.cpp \
    takealook/imagehandler.cpp \
    takealook/imageproperties.cpp \
    takealook/toggleaction.cpp \
    takealook/baloontip/baloontip.cpp \
    takealook/baloontip/pixbaloontip.cpp \
    takealook/utility/icons.cpp \
    takealook/utility/mouseevent.cpp \
    takealook/utility/pathutilities.cpp \

HEADERS += \
    takealook/mainwindow.h \
    takealook/scene.h \
    takealook/view.h \
    takealook/areaselectionitem.h \
    takealook/imagehandler.h \
    takealook/imageproperties.h \
    takealook/toggleaction.h \
    takealook/baloontip/baloontip.h \
    takealook/baloontip/pixbaloontip.h \
    takealook/utility/icons.h \
    takealook/utility/mouseevent.h \
    takealook/utility/pathutilities.h \

FORMS += \
    takealook/mainwindow.ui

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
