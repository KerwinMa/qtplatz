# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = adportable
DESTDIR = ../../../lib/qtPlatz
CONFIG += staticlib
DEFINES += QT_LARGEFILE_SUPPORT IDE_LIBRARY_BASENAME=\"lib\" WITH_TESTS QT_NO_CAST_TO_ASCII QT_TESTLIB_LIB
INCLUDEPATH += $(BOOST_INCLUDE) \
    ./../../libs \
    $(ACE_ROOT) \
    ./GeneratedFiles/Debug \
    ./../../libs/xmlwrapper
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(adportable.pri)
