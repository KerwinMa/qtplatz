#-------------------------------------------------
#
# Project created by QtCreator 2010-07-10T13:47:50
#
#-------------------------------------------------

#QT       -= gui

TARGET = qtwrapper
TEMPLATE = lib
CONFIG += staticlib
include(../../qtplatzstaticlib.pri)

SOURCES += qtwrapper.cpp \
    qstring.cpp \
    application.cpp \
    xmlformatter.cpp \ 
    progressbar.cpp

HEADERS += qtwrapper.hpp \
    qstring.hpp \
    application.hpp \
    xmlformatter.hpp \ 
    waitcursor.hpp \
    progressbar.hpp \
    trackingenabled.hpp
