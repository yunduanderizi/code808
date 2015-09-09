#-------------------------------------------------
#
# Project created by QtCreator 2012-03-19T14:16:18
#
#-------------------------------------------------

include(../product.conf)

system(chmod -x *.h)

TARGET = radioManager
TEMPLATE = lib

INCLUDEPATH +=  ./si475x
INCLUDEPATH +=  ./tda7703

SOURCES += \
    si475x/si475xFMRX.c \
    si475x/si475x_low.c \
    si475x/io.c \
    si475x/si47xxFMRX.c \
    radioManager.cpp \
    si475x/si47xxAMRX.c \
    si475x/si475xAMRX.c \
    si475x/radioManagerOld.cpp \
    si475x/si4754radio.cpp \
    si475x/asc1411.c \
    tda7703/gpio.c \
    tda7703/radio_hit.c \
    tda7703/radioManager_TDA7703.cpp \
    radioCommon.c

HEADERS += \
    si475x/typedefs.h \
    si475x/si475xFMRX.h \
    si475x/propertydefs.h \
    si475x/io.h \
    si475x/commanddefs.h \
    radioManager.h \
    si475x/si47xxFMRX.h \
    si475x/si47xxAMRX.h \
    si475x/si475xAMRX.h \
    si475x/radioManagerOld.h \
    abstractRadio.h \
    si475x/si4754radio.h \
    si475x/asc1411.h \
    tda7703/PUBLIC.H \
    tda7703/radio_hit.h \
    tda7703/radioManager_TDA7703.h \
    radioDefines.h \
    radioCommon.h

QMAKE_POST_LINK = make install

install_include.files += radioManager.h radioDefines.h
install_include.path = $$INNOVBOX_RADIOMANAGER_8925_INSTALL_DIR/include

target.path = $$INNOVBOX_RADIOMANAGER_8925_INSTALL_DIR/lib

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/*  $$target.path/*




