#-------------------------------------------------
#
# Project created by QtCreator 2010-06-27T16:15:52
#
#-------------------------------------------------

QT       -= core gui
QT  += xml

TARGET = xmlparser
TEMPLATE = lib
CONFIG += staticlib

include(../../qtplatz_lib_static.pri)

SOURCES +=  pugiwrapper.cpp

HEADERS +=  pugiwrapper.h

OTHER_FILES += \
    xmlparser.pri
