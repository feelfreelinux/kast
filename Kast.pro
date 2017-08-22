QT += core network gui

TARGET = kast

CONFIG += console c++11 warn_on
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/DLNARenderer.cpp \
    src/HTTPFileServer.cpp \
    src/HTTPFileClient.cpp \
    src/Kast.cpp \
    src/SOAPActionManager.cpp \
    src/SSDPDiscovery.cpp \
    src/MimeGuesser.cpp \
    src/Thumbnailer/imagewriter.cpp \
    src/Thumbnailer/moviedecoder.cpp \
    src/Thumbnailer/videothumbnailer.cpp \
    src/ThumbnailCreator.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include

HEADERS += \
    include/DLNARenderer.h \
    include/HTTPFileServer.h \
    include/HTTPFileClient.h \
    include/SSDPDiscovery.h \
    include/SOAPActionManager.h \
    include/Kast.h \
    include/DLNAPlaybackInfo.h \
    include/DLNARendererIcon.h \
    include/MimeGuesser.h \
    include/Thumbnailer/histogram.h \
    include/Thumbnailer/imagewriter.h \
    include/Thumbnailer/moviedecoder.h \
    include/Thumbnailer/videoframe.h \
    include/Thumbnailer/videothumbnailer.h \
    include/ThumbnailCreator.h

unix|win32: LIBS += -lavcodec -lavutil -lavformat -lavfilter -lswscale
