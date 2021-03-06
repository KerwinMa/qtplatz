#-------------------------------------------------
#
# Project created by QtCreator 2010-08-14T07:16:37
#
#-------------------------------------------------

QT += core gui svg printsupport

TARGET = qtwidgets2
TEMPLATE = lib

PROVIDER = MS-Cheminformatics
include(../../adplugin.pri)
include(../../boost.pri)
include( ../../ace_tao.pri )

LIBS += -l$$qtLibraryTarget(adportable) -l$$qtLibraryTarget(adcontrols) \
    -l$$qtLibraryTarget(adplugin) -l$$qtLibraryTarget(qtwrapper) \
    -l$$qtLibraryTarget(xmlparser)

!win32 {
  LIBS *= -lboost_filesystem -lboost_system
}

INCLUDEPATH *= $(ACE_ROOT) $(TAO_ROOT)

#workaround for 5.1.0 cause a fatal error on qitemdelegate.h can not find qabstractitemdelegate.h
greaterThan( QT_MAJOR_VERSION, 4 ): INCLUDEPATH += ${QTDIR}/include  

DEFINES += QTWIDGETS2_LIBRARY

SOURCES += factory.cpp \
        mscalibratedelegate.cpp \
        mscalibrationform.cpp \
        mscalibsummarydelegate.cpp \
        mscalibsummarywidget.cpp \
        standarditemhelper.cpp \
        tableview.cpp \
        qtwidgets2.cpp

HEADERS += \
        factory.hpp \
        mscalibratedelegate.hpp \
        mscalibrationform.hpp \
        mscalibsummarydelegate.hpp \
        mscalibsummarywidget.hpp \
        standarditemhelper.hpp \
        tableview.hpp \
        qtwidgets2.hpp

FORMS += \
    mscalibrationform.ui

RESOURCES +=

