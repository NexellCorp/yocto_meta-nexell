#
# Make options
#
#	CROSS_COMPILE	: define cross compiler (default arm-linux-gnueabihf-gcc)
#	DEBUG=y		: use debug build
#	LIB_DIR=<path>	: set library path
#	ALSA_ASOUND=y	: use alsa asound library (default tiny alsa)
#

CROSS_COMPILE := arm-linux-gnueabihf-

ALSA_ASOUND :=
AEC_PCMOUT  :=
LIB_DIR := ../lib

CC  = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++

CFLAGS  :=  -Wall -std=c++11
CFLAGS  +=  -mfloat-abi=hard

ifeq ($(AEC_PCMOUT),y)
CFLAGS  += -DSUPPORT_AEC_PCMOUT
endif

LDFLAGS  +=  -mfloat-abi=hard

ifeq ($(DEBUG),y)
CFLAGS  += -DDEBUG
CFLAGS  += -g
CFLAGS  += -fno-omit-frame-pointer
CFLAGS  += -fstack-protector-all
LDFLAGS += -rdynamic # '-rdynamic' or '-Wl,export-dynamic'
LDFLAGS += -static
CFLAGS  += -I $(LIB_DIR)
else
CFLAGS  += -O2
CFLAGS  += -fno-omit-frame-pointer
CFLAGS  += -I $(LIB_DIR)
endif

ifneq ($(ALSA_ASOUND),y)
CFLAGS  += -DTINY_ALSA
endif

ifeq ($(AEC_PCMOUT),y)
LIBS	:= $(LIB_DIR)/libPreproc1.a
endif

LIBS	+= $(LIB_DIR)/libtinyalsa.a
LIBS	+= $(LIB_DIR)/libagcpdm.a
LIBS	+= $(LIB_DIR)/libresample.a
LIBS	+= -lpthread
LIBS	+= -lm
ifeq ($(ALSA_ASOUND),y)
LIBS	+= -lasound
endif

COBJS	:= util.o
CPPOBJS	:= smart_voice.o audioplay.o audiostream.o
OBJS	:= $(COBJS) $(CPPOBJS)

TARGET = smart_voice

all : $(TARGET)

$(TARGET) : depend $(OBJS)
	$(CPP) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

.PHONY: clean

clean :
	rm -rf $(OBJS) $(TARGET) core .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

SRCS := $(COBJS:.o=.c) $(CPPOBJS:.o=.cpp)
depend dep:
	$(CC)  -M $(CFLAGS) $(SRCS) > .depend

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

%.o: %.cpp
	$(CPP) $(CFLAGS) $(INCLUDE) -c -o $@ $<
