TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS   = \
    aggregation \
    extensionsystem \
    utils \
    adportable \
    adextension \
    adinterface \
    portfolio \
    adutils \
    acewrapper \
    adcontrols \
    chromatogr \
    xmlparser \
    adtxtfactory \
    adfs \
    addatafile \
    adwplot \
    qtwrapper \
    adplugin \
    qtwidgets \
    adchem \
    adsequence

# adfs : boost
# xmlparser : none
# adinterface : ACE TAO
# acewrapper : boost
# adportable : acewrapper
# adutils : boost
# adwplot : QWT
# qtwrapper : QtCore

# adplugin (dll) : ACE, TAO
# adcontroller (dll) : adinterface, adportable, acewrapper, xmlparser, adplugin
# qtwidgets (dll) : adcontrols
# adwidgets (dll) : adcontrols
# adborker (dll) : adinterface, adportable, acewrapper adcontrols portfolio
# adtxtfactory (dll) : adcontrols, adportable, xmlparser, portfolio
