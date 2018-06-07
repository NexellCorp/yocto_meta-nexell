#
# Make options
#
#	CROSS_COMPILE	: define cross compiler (default arm-linux-gnueabihf-gcc)
#	DEBUG=y		: use debug build
#

CROSS_COMPILE := arm-linux-gnueabihf-

CC  = $(CROSS_COMPILE)gcc
STRIP  = $(CROSS_COMPILE)strip
CFLAGS  := -Wall -O3
#CFLAGS  += -Werror
#CFLAGS  += -Wno-implicit-function-declaration

#LDFLAGS := -static
LIBS	:= -lpthread -ldl

ifeq ($(DEBUG),y)
CFLAGS  += -DDEBUG
endif

COBJS	:= qtbootlauncher.o
OBJS	:= $(COBJS)

TARGET = qtbootlauncher

all : $(TARGET)

$(TARGET) : depend $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)
	$(STRIP) $@

.PHONY: clean

clean :
	rm -rf $(OBJS) $(TARGET) core .depend

distclean : clean

ifeq (.depend,$(wildcard .depend))
include .depend
endif

SRCS := $(COBJS:.o=.c)
depend dep:
	$(CC)  -M $(CFLAGS) $(SRCS) > .depend

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)
