#CC=gcc
#CXX=g++
#CC = arm-none-linux-gnueabi-gcc
CXX= arm-none-linux-gnueabi-g++
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
objects = io.o mcuCommon.o update_mcu.o ib_i2cctl.o
#AR=ar
#all:libjt808.a
update_mcu:$(objects)
	$(CXX) -o update_mcu io.o mcuCommon.o update_mcu.o ib_i2cctl.o -pthread

io.o:io.cpp io.h 
	$(CXX) -c io.cpp 
mcuCommon.o:mcuCommon.cpp mcuCommon.h
	$(CXX) -c mcuCommon.cpp 
update_mcu.o:update_mcu.cpp update_mcu.h
	$(CXX) -c update_mcu.cpp 

ib_i2cctl.o:ib_i2cctl.h ib_i2cctl.cpp
	$(CXX) -c ib_i2cctl.cpp

.PHONY:clean
clean:
	rm -f *.o update_mcu
