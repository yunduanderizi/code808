include(../product.conf)

system(chmod -x *.h)


TEMPLATE = app
TARGET = JT808Manager

QT +=  core xml

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include
INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$INNOVBOX_GB19056_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_BUSDAEMON_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_INSTALL_DIR/audioManager/include
INCLUDEPATH += $$INNOVBOX_INSTALL_DIR/wm/include
INCLUDEPATH += $$INNOVBOX_SRC_DIR/second_jt808

LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus
LIBS += -L$$PACKAGE_TSLIB_DIR/lib 
LIBS += -L$$INNOVBOX_GB19056_INSTALL_DIR/lib -lgb19056

# Input
HEADERS += ConfigManager.h \
           JT808MessageHandler.h JT808MessageHelper.h \
           JT808DbusMessage.h JT808DbusMessageHandler.h \
           ../common/message/NaviDbusMessage.h\
			McuMessageHandler.h\
			McuProtocol.h\
			JT808GnssManager.h
SOURCES += ConfigManager.cpp \
           JT808MessageHandler.cpp JT808MessageHelper.cpp \
           JT808Manager.cpp JT808DbusMessageHandler.cpp \
           ../common/message/NaviDbusMessage.cpp\ 
			McuMessageHandler.cpp\
			McuProtocol.cpp\
			JT808GnssManager.cpp
CONFIG(CONNECTION_MANAGER_AT) {
message(Build connection manager with AT)
HEADERS += ConnectionManagerAT.h
SOURCES += ConnectionManagerAT.cpp    
} else {
message(Build standard connection manager)
HEADERS += ConnectionManagerSTD.h
SOURCES += ConnectionManagerSTD.cpp    
}


QMAKE_POST_LINK =make install

install_include.files = JT808DbusMessage.h

install_include.path  = $$INNOVBOX_JT808_INSTALL_DIR/include

target.path  = $$INNOVBOX_JT808_INSTALL_DIR

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/* $$target.path/*
