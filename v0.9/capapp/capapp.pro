include(../product.conf)

system(chmod -x *.h $$INNOVBOX_SRC_DIR/common/message/*.h)

TEMPLATE = app
TARGET = capapp
DEPENDPATH += . include


INCLUDEPATH += $$PACKAGE_OPENCV_DIR/include
INCLUDEPATH += $$PACKAGE_DBUS_DIR/include 
INCLUDEPATH += $$INNOVBOX_COMMON_SRC_DIR/message
INCLUDEPATH += $$PACKAGE_BOOST_DIR/include 
INCLUDEPATH += $$PACKAGE_SQLITE_DIR/include
INCLUDEPATH += $$INNOVBOX_WM_INSTALL_DIR/include

INCLUDEPATH += $$INNOVBOX_UTILITY_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_TABLES_INSTALL_DIR/include
INCLUDEPATH += $$INNOVBOX_JT808_INSTALL_DIR/include 

INCLUDEPATH += .include
INCLUDEPATH += $$PACKAGE_OPENCV_DIR/include


TARGET_CPU =cortex-a5
TARGET_ARCH =armv7-a

#QMAKE_CFLAGS	+= -mcpu= cortex-a5
#QMAKE_CXXFLAGS	+= -mcpu= cortex-a5
QMAKE_AFLAGS	= -march=armv7-a -EL 
QMAKE_CFLAGS 	+=-msoft-float -mfpu=vfp -lm

DEFINES +=  TCC_89XX_INCLUDE
#DEFINES +=  TW2826_SENSOR
QMAKE_CFLAGS  += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_REENTRANT

INCLUDEPATH +=  -I$(CROSS_COMPILE_PATH)codesourcery/arm-none-linux-gnueabic/libc/usr/include 
INCLUDEPATH +=  -I$(CROSS_COMPILE_PATH)codesourcery/arm-none-linux-gnueabic/include

LIBS += -L$(CROSS_COMPILE_PATH)codesourcery/arm-none-linux-gnueabi/libc/usr/lib
LIBS += -L$(CROSS_COMPILE_PATH)codesourcery/arm-none-linux-gnueabi/libc/lib



LIBS += -L$$PACKAGE_OPENCV_DIR/lib -ljpeg -lcv -lhighgui -lcvaux -lml -lz -lcxcore  -lpthread -lm -lrt
LIBS += -L$$PACKAGE_DBUS_DIR/lib -ldbus-1 -lQtDBus -lQtXml
LIBS += -L$$PACKAGE_SQLITE_DIR/lib -lsqlite3
LIBS += -L$$PACKAGE_BOOST_DIR/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options -lboost_locale
LIBS += -L$$INNOVBOX_UTILITY_INSTALL_DIR/lib -lutility
LIBS += -L$$INNOVBOX_TABLES_INSTALL_DIR/lib -ltables
LIBS += -L$$INNOVBOX_WM_INSTALL_DIR/lib -lwm
LIBS += -L. -lTCC892x_MP4MUX_LINUX_V1.7.11.0
LIBS += -L$$PACKAGE_TSLIB_DIR/lib

# Input
HEADERS += av_common_fcc_tag.h \
	av_common_mux.h \
	av_common_types.h \
	cdk_core.h \
	cdk_error.h \
	cdk.h \
	cdk_port.h \
	cdk_pre_define.h \
	encoder.h \
	mp4_muxer.h \
	muxer.h \
	pmap.h \
	scalejpeg.h \
	TCC892x_VPU_CODEC.h \
	tcc_cam_ioctrl.h \
	TCC_CODA960_CODEC.h \
	tccfb_ioctl_code.h \
	tcc_gre2d_type.h \
	tcc_grp_ioctrl.h \
	tcc_image_common.h \
	tcc_linux_system.h \
	TCC_MP4_muxer.h \
	tcc_overlay_interface.h \
	tcc_overlay_ioctl.h \
	tcc_overlaymix.h \
	tcc_scaler_interface.h \
	tcc_scaler_ioctrl.h \
	TCC_TS_MUXER.h \
	tcc_v4l2.h \
	tcc_v4l2_ioctl.h \
	TCC_VPU_CODEC.h \
	tcc_vpu_encode_interface.h \
	tcc_vpu_ioctl.h \
	TCCxxxx_VPU_CODEC_COMMON.h \
	ts_muxer.h \
	venc.h \
	videodev2.h \
	yuv2jpg.h \
	queue.h	\
         
SOURCES +=capapp.cpp \ 
	pmap.c \
	TCC_MP4_muxer.c \
	venc_k.c \
	encoder.c \
	tcc_overlaymix.c \
	tcc_scaler_interface.c \
	tcc_overlay_interface.c \
	tcc_linux_system.c \
	tcc_v4l2.c \
	tcc_v4l2_ioctl.c \
	scalejpeg.cpp \
	yuv2jpg.c \
	queue.c \
	main.c \

QMAKE_POST_LINK =make install

target.path  = $$INNOVBOX_CAPAPP_INSTALL_DIR

install_include.files = $$INNOVBOX_COMMON_SRC_DIR/message/CapappDBusMessage.h

install_include.path  = $$INNOVBOX_CAPAPP_INSTALL_DIR/include

INSTALLS += target install_include

QMAKE_DISTCLEAN += $$target.path/* $$install_include.path/*

