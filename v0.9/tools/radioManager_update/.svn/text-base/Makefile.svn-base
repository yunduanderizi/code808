#CC=gcc
CC = arm-none-linux-gnueabi-gcc
#CFLAGS = -c -static -I../../../../trunk/packages/target/libboost_1_49_0/include\
	 -I../../../../trunk/packages/target/sqlite3/include\
	 -I../../innov/utility/include\
	 -I../../innov/table/include\
	 -I../../innov/gb19056/include
#LIBS = -L../../../../trunk/packages/target/libboost_1_49_0/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options\
       -lboost_locale\
       -L../../../../trunk/packages/target/sqlite3/lib\
       -L../../innov/utility/lib -lutility\
       -L../../innov/table/lib -ltables\
       -L../../innov/gb19056/lib -lgb19056\
#       -lpthread -lrt  
objects = io.o radioCommon.o update_radio.o 
#AR=ar
#all:libjt808.a
update_radio:$(objects)
	$(CC) -o update_radio io.o radioCommon.o update_radio.o -pthread

io.o:io.c io.h 
	$(CC) -c io.c 
radioCommon.o:radioCommon.c radioCommon.h
	$(CC) -c radioCommon.c 
update_radio.o:update_radio.c update_radio.h
	$(CC) -c update_radio.c 

.PHONY:clean
clean:
	rm -f *.o update_radio
