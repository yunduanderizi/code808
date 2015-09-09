#-------------------------------------------------
#
# Project created by QtCreator 2013-12-11T21:21:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Navi
TEMPLATE = app
unix:DEFINES           += _TTY_POSIX_

#platform linux
linux-g++{

INCLUDEPATH += $$PWD/common/include

INCLUDEPATH += $$PWD/Platform/visual-sim
INCLUDEPATH += $$PWD/Platform
INCLUDEPATH += $$PWD/Platform/include
INCLUDEPATH += $$PWD/hmi/include
INCLUDEPATH += $$PWD/gps/include
INCLUDEPATH += $$PWD/control

PRE_TARGETDEPS += $$PWD/linux/lib/rel/libinnovengine.a $$PWD/linux/lib/rel/libtremor.a $$PWD/linux/lib/rel/libz.a
LIBS += -lQtDBus -lQtCore -lQtXml -L$$PWD/linux/lib/rel/ -linnovengine -ltremor -lz \
-L$$PWD/wm/lib/ -lpthread -ldl


SOURCES += main.cpp\
    platform.cpp \
    csingalobject.cpp \
    hmi/navibasewnd.cpp \
    engineinterface/engineinterface.cpp \
    engineinterface/filenamelist.cpp \
    engineinterface/navcoreFavourites.cpp \
    engineinterface/navcoreGraphics.cpp \
    engineinterface/navcoreJourney.cpp \
    engineinterface/navcoreDisplay.cpp \
    engineinterface/navcore.cpp \
    gps/posix_qextserialport.cpp \
    gps/qextserialbase.cpp

HEADERS  += system_param.h \
    csingalobject.h \
    hmi/navibasewnd.h \
    engineinterface/car.h \
    engineinterface/engineinterface.h \
    engineinterface/filenamelist.h \
    engineinterface/navcoreFavourites.h \
    engineinterface/navcoreGraphics.h \
    engineinterface/navcoreJourney.h \
    engineinterface/system_param.h \
    gps/posix_qextserialport.h \
    gps/qextserialbase.h


FORMS    +=

}else{
DEFINES += ARM_LINUX
#platform armlinux
INCLUDEPATH += $$PWD/common/include


INCLUDEPATH += $$PWD/Platform/visual-sim
INCLUDEPATH += $$PWD/Platform
INCLUDEPATH += $$PWD/Platform/include
INCLUDEPATH += $$PWD/dbus/include
INCLUDEPATH += $$PWD/wm/include
INCLUDEPATH += $$PWD/hmi/include
INCLUDEPATH += $$PWD/gps/include
INCLUDEPATH += $$PWD/engineinterface/include
INCLUDEPATH += $$PWD/dbus/gpsinterface/include
INCLUDEPATH += $$PWD/dbus/fence/table/include
INCLUDEPATH += $$PWD/inputmethod/include
INCLUDEPATH += $$PWD/TTS/aiTTS/Inc
INCLUDEPATH += $$PWD/TTS/audiocontrolcenter/include
INCLUDEPATH += $$PWD/alsa/
INCLUDEPATH += $$PWD/alsa/sound
INCLUDEPATH += $$PWD/control

PRE_TARGETDEPS += $$PWD/arm/lib/rel/libinnovengine.a $$PWD/arm/lib/rel/libtremor.a $$PWD/arm/lib/rel/libz.a
LIBS += -lQtDBus -lQtCore -lQtXml -L$$PWD/arm/lib/rel/ -linnovengine -ltremor -lz \
-L$$PWD/TTS/aiTTS/Lib -lAiSound5 \
-L$$PWD/dbus/lib/ -ldbus-1 -L$$PWD/wm/lib/ -lwm $$PWD/libboost_1_49_0/lib/libboost_timer.so.1.49.0 $$PWD/libboost_1_49_0/lib/libboost_system.so.1.49.0 $$PWD/libboost_1_49_0/lib/libboost_chrono.so.1.49.0\
$$PWD/dbus/gpsinterface/lib/libgpsinterface.so.1.0.0 $$PWD/dbus/fence/table/lib/libtables.so.1.0.0 \
$$PWD/dbus/utility/lib/libutility.so.1.0.0 \
$$PWD/inputmethod/lib/libinputmethod.so $$PWD/inputmethod/lib/libinputmethod.so.1 $$PWD/inputmethod/lib/libinputmethod.so.1.0 $$PWD/inputmethod/lib/libinputmethod.so.1.0.0 \
$$PWD/TTS/audiocontrolcenter/lib/libaudioControl.so $$PWD/TTS/audiocontrolcenter/lib/libaudioControl.so.1 $$PWD/TTS/audiocontrolcenter/lib/libaudioControl.so.1.0  $$PWD/TTS/audiocontrolcenter/lib/libaudioControl.so.1.0.0 \
$$PWD/TTS/alsa/lib/libasound.so $$PWD/TTS/alsa/lib/libasound.so.2 $$PWD/TTS/alsa/lib/libasound.so.2.0.0


SOURCES += main.cpp\
    platform.cpp \
    csingalobject.cpp \
    hmi/navibasewnd.cpp \
    engineinterface/engineinterface.cpp \
    engineinterface/filenamelist.cpp \
    engineinterface/navcoreFavourites.cpp \
    engineinterface/navcoreGraphics.cpp \
    engineinterface/navcoreJourney.cpp \
    engineinterface/navcore.cpp \
    engineinterface/navcoreDisplay.cpp \
    gps/posix_qextserialport.cpp \
    gps/qextserialbase.cpp \
    dbus/NaviDbusMessage.cpp \
    dbus/dbusinterface.cpp

HEADERS  += system_param.h \
    csingalobject.h \
    hmi/navibasewnd.h \
    engineinterface/car.h \
    engineinterface/engineinterface.h \
    engineinterface/filenamelist.h \
    engineinterface/navcoreFavourites.h \
    engineinterface/navcoreGraphics.h \
    engineinterface/navcoreJourney.h \
    engineinterface/system_param.h \
    engineinterface/navcore.h \
    engineinterface/navcoreDisplay.h \
    gps/posix_qextserialport.h \
    gps/qextserialbase.h \
    dbus/NaviDbusMessage.h \
    dbus/dbusinterface.h

FORMS    +=
}

SOURCES += \
    TTS/read_news.cpp \
    math/mathmatics.cpp \
    fence/fenceunit.cpp \
    math/enworld.c \
    gps/gps_parse.cpp \
    control/uipicture.cpp \
    control/uibutton.cpp \
    control/uiresultlist.cpp \
    control/uitext.cpp \
    gps/bd_parse.cpp \
    gps/gn_parse.cpp \
    gps/satellitemodule.cpp \
    fence/fencemanager.cpp \
    control/uilabel.cpp \
    windowinterface.cpp \
    TTS/platformTTS.cpp \
    hmi/menu_routemain.cpp \
    hmi/menu_routeheadclose.cpp \
    hmi/menu_routeareaclose.cpp \
    hmi/menu_routestrategy.cpp \
    hmi/msgbox_twobtn.cpp \
    hmi/menu_routelistclose.cpp \
    engineinterface/interface_route.cpp \
    engineinterface/interface_map.cpp \
    hmi/map_laneinfo.cpp \
    engineinterface/interface_guide.cpp \
    hmi/searchthread.cpp \
    hmi/map_areacloseedit.cpp \
    hmi/map_junctionview.cpp \
    hmi/menu_setmapmode.cpp \
    hmi/menu_setsound.cpp \
    hmi/map_signpost.cpp \
    engineinterface/interface_search.cpp \
    engineinterface/interface_logistics.cpp \
    engineinterface/navcoreRoute.cpp \
    hmi/menu_setgpsmode.cpp \
    hmi/menu_searchkeyword.cpp \
    hmi/menu_searchcross.cpp \
    hmi/menu_searchcategory.cpp \
    hmi/menu_searcharound.cpp \
    hmi/menu_searchadminprovince.cpp \
    hmi/menu_searchadmincity.cpp \
    hmi/menu_setup.cpp \
    hmi/menu_search.cpp \
    hmi/menu_searchresultlist.cpp \
    hmi/menu_searchresultlistcategory.cpp \
    hmi/menu_searchresultinfo.cpp \
    hmi/msgbox_onebtn.cpp \
    hmi/menu_setversion.cpp \
    hmi/menu_routeconfirm.cpp \
    hmi/menu_searchpoiview.cpp \
    hmi/map_guide.cpp \
    hmi/msgbox_threebtn.cpp \
    hmi/map_broswer.cpp \
    hmi/map_mainwindow.cpp

HEADERS += \
    engineinterface/navcoreFence.h \
    inputmethod/InputInit.h \
    inputmethod/InputMethod.h \
    TTS/read_news.h \
    TTS/aiTTS/Inc/ivTTSSDKID.h \
    TTS/aiTTS/Inc/ivTTS.h \
    TTS/aiTTS/Inc/ivDefine.h \
    TTS/aiTTS/Inc/ivPlatform.h \
    TTS/audiocontrolcenter/include/audio_main_volume.h \
    TTS/audiocontrolcenter/include/audio_control.h \
    TTS/audiocontrolcenter/include/audio_control_server.h \
    TTS/audiocontrolcenter/include/audio_control_client.h \
    alsa/asoundlib.h \
    math/mathmatics.h \
    fence/fenceunit.h \
    math/enworld.h \
    fontset.h \
    gps/gps_parse.h \
    control/uipicture.h \
    control/uibutton.h \
    control/uiresultlist.h \
    gps/bd_parse.h \
    gps/gn_parse.h \
    gps/satellitemodule.h \
    fence/fencemanager.h \
    control/uilabel.h \
    control/uitext.h \
    windowinterface.h \
    TTS/platformTTS.h \
    globaldefine.h \
    engineinterface/goldstar.h \
    hmi/menu_routemain.h \
    hmi/menu_routeheadclose.h \
    hmi/menu_routeareaclose.h \
    hmi/menu_routestrategy.h \
    hmi/msgbox_twobtn.h \
    hmi/menu_routelistclose.h \
    engineinterface/interface_route.h \
    engineinterface/interface_map.h \
    hmi/map_laneinfo.h \
    engineinterface/interface_guide.h \
    hmi/searchthread.h \
    hmi/map_areacloseedit.h \
    hmi/map_junctionview.h \
    hmi/menu_setmapmode.h \
    hmi/menu_setsound.h \
    hmi/map_signpost.h \
    engineinterface/interface_search.h \
    engineinterface/interface_logistics.h \
    engineinterface/datatype.h \
    engineinterface/navcoreRoute.h \
    hmi/menu_setgpsmode.h \
    hmi/menu_searchkeyword.h \
    hmi/menu_searchcross.h \
    hmi/menu_searchcategory.h \
    hmi/menu_searcharound.h \
    hmi/menu_searchadminprovince.h \
    hmi/menu_searchadmincity.h \
    hmi/map_mainwindow.h \
    hmi/menu_setup.h \
    hmi/menu_search.h \
    hmi/menu_searchresultlist.h \
    hmi/menu_searchresultlistcategory.h \
    hmi/menu_searchresultinfo.h \
    hmi/msgbox_onebtn.h \
    hmi/menu_setversion.h \
    hmi/menu_routeconfirm.h \
    hmi/menu_searchpoiview.h \
    hmi/map_guide.h \
    hmi/msgbox_threebtn.h \
    hmi/map_broswer.h

FORMS += \
    menu_setting040000.ui \
    menu_aroundsearch010200.ui \
    menu_CategorySearch010400.ui \
    menu_infoview020000.ui \
    menu_keywordSearch010100.ui \
    menu_SubListSearch010101.ui \
    menu_Search010000.ui \
    menu_adminmenu01010101.ui \
    menu_resultlistmenu01010100.ui \
    confirmDialog.ui \
    map_infoview040200.ui \
    menu_routemenu070000.ui \
    menu_headclose070200.ui \
    menu_AreaClose070300.ui \
    menu_routeStrategy070100.ui \
    msg_twobtn.ui \
    menu_routelistclose070400.ui \
    map_laneinfo.ui \
    map_AreaCloseEdit070401.ui \
    menu_settingMapmode040600.ui \
    menu_settingSound040200.ui \
    map_junctionview.ui \
    map_signpost.ui \
    menu_settingGpsmode040800.ui \
    menu_settingVersion040700.ui \
    map_guide010200.ui \
    map_broswer080000.ui \
    basemap000000.ui \
    msg_onebtn.ui \
    msg_threebtn.ui \
    map_routeconfirm070900.ui \
    menu_satellite040500.ui

OTHER_FILES +=

RESOURCES +=






