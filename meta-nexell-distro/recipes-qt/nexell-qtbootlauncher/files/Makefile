#
# Make options
#
# DEBUG=y		: use debug build
#

CFLAGS  := -Wall -O3
LIBRARY	:= -lpthread -ldl

ifeq ($(DEBUG),y)
CFLAGS  += -DDEBUG
endif

LDFLAGS += -static

COBJS	:= nx_qtbootlauncher.o
OBJS	:= $(COBJS)

TARGET = nx_qtbootlauncher

all : $(TARGET)

$(TARGET) : depend $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBRARY)

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
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBRARY)
