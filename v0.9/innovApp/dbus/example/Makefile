
DBUS_DIR      =..
QTDIR_DIR     =/home/bai/dbus_emd/qt462


CLIENT_INCL  	=-c -pipe -march=armv6 -O2 -Wall -W -D_REENTRANT -I. -I$(DBUS_DIR)/include
CLIENT_LFLAGS  	=-Wl,-O1  
CLIENT_LIBS  	=-L$(DBUS_DIR)/lib -lQtDBus -ldbus-1 -L$(QTDIR_DIR)/lib -lQtXml -lQtCore -L$(DBUS_DIR)/lib -lts -lpthread

SERVER_INCL  	=-c -pipe -march=armv6 -O2 -Wall -W -D_REENTRANT -I. -I$(DBUS_DIR)/include -I$(QTDIR_DIR)/include
SERVER_LFLAGS  	=-Wl,-O1   
SERVER_LIBS  	=-L$(DBUS_DIR)/lib -lQtDBus -ldbus-1 -L$(QTDIR_DIR)/lib -lQtXml -lQtCore -lQtGui -lQtNetwork -L$(DBUS_DIR)/lib -lts -lpthread

TARGET1        = server
TARGET2        = client 

all: $(TARGET1) $(TARGET2)

$(TARGET1):  serverMain.o  testMessage.o
	arm-none-linux-gnueabi-g++  $(SERVER_LFLAGS) -o $@ $+ $(SERVER_LIBS)

serverMain.o: serverMain.cpp testMessage.h
	arm-none-linux-gnueabi-g++ $(SERVER_INCL) -o $@ serverMain.cpp

$(TARGET2):  clientMain.o  testMessage.o
	arm-none-linux-gnueabi-g++ $(CLIENT_LFLAGS) -o $@ $+ $(CLIENT_LIBS)

clientMain.o: clientMain.cpp testMessage.h
	arm-none-linux-gnueabi-g++ $(CLIENT_INCL) -o $@ clientMain.cpp

testMessage.o: testMessage.cpp testMessage.h
	arm-none-linux-gnueabi-g++ $(CLIENT_INCL) -o $@ testMessage.cpp

clean:
	rm -f  *.o $(TARGET1) core $(TARGET2)

