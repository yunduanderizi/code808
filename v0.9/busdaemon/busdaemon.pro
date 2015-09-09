include(../product.conf)

system(chmod -x *.h)


TEMPLATE = app
TARGET = busdaemon

QT +=  core xml

INCLUDEPATH += $$PACKAGE_DBUS_DIR/include
INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_DEVICEMANAGER_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$INNOVBOX_DEVICEMANAGER_SRC_DIR

LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus
LIBS += -L$$PACKAGE_TSLIB_DIR/lib 

# Input
HEADERS += busmessage.h SerialConnectionManager.h McuMessageHandler.h GpsMessageHandler.h GpsLocationMessageHandler.h

SOURCES += busdaemon.cpp SerialConnectionManager.cpp McuMessageHandler.cpp GpsMessageHandler.cpp GpsLocationMessageHandler.cpp busmessage.cpp

QMAKE_POST_LINK =make install

install_include.files = busmessage.h

install_include.path  = $$INNOVBOX_BUSDAEMON_INSTALL_DIR/include

target.path  = $$INNOVBOX_BUSDAEMON_INSTALL_DIR

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$install_include.path/* $$target.path/*

