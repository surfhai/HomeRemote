TEMPLATE = app

QT += qml quick network xml
CONFIG += c++11

SOURCES += main.cpp \
    kodi.cpp \
    kodipvrchannel.cpp \
    kodipvrchannelmodel.cpp \
    globals.cpp \
    homematic.cpp \
    hmdevice.cpp \
    gethtml.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    kodi.h \
    xbmcclient.h \
    kodipvrchannel.h \
    kodipvrchannelmodel.h \
    utils.h \
    globals.h \
    homematic.h \
    hmdevice.h \
    gethtml.h

DISTFILES += \
    Clock.qml \
    Navigation.qml \
    ImgButton.qml \
    KodiPVRChannelsDelegate.qml \
    ScrollBar.qml

include(libs/libmaia/maia.pri)
