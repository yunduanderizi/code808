#DEFINES = -DLARGE_PICTURE

CLIENT_INCL  	=-c -pipe -march=armv6 -O2 -Wall -W -D_REENTRANT -I. $(DEFINES) 
CLIENT_LFLAGS  	=-Wl,-O1 $(DEFINES) 
CLIENT_LIBS  	=

TARGET        = startui 

all: $(TARGET) 

$(TARGET):  main.o  
	arm-none-linux-gnueabi-g++ $(CLIENT_LFLAGS) -o $@ $<

main.o: main.cpp 
	arm-none-linux-gnueabi-g++ $(CLIENT_INCL) -o $@ $<

clean:
	rm -f  *.o $(TARGET) core 

