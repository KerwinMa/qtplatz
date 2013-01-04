#-------------------------------------------------
#
# Project created by QtCreator 2013-01-04T16:30:19
#
#-------------------------------------------------

QT       -= gui

TARGET = adextension
TEMPLATE = lib

include(../../qtplatz_library.pri)

DEFINES += ADEXTENSION_LIBRARY

SOURCES += adextension.cpp \
           isequence.cpp \
    ieditorfactory.cpp

HEADERS += adextension.hpp\
           adextension_global.hpp \
           isequence.hpp \
    ieditorfactory.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}