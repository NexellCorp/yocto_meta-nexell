.SUFFIXES : .c .o

CFLAGS += -DPSPLASH_UI

TARGET = nx_update

OBJS = 	main.o \
		proceed_update_cmd.o \
		util.o \
		erase.o \
		flash.o \
		spieeprom.o \
		fdisk.o \
		ext4.o

SRCS := $(OBJS:.o=.c)
INCS := $(INCLUDE)

$(TARGET): depend $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	@find $(OBJTREE) -type f \
		\( -name '*.o'  -o -name $(TARGET) -o -name $(TARGET2) -o -name '*.map' \) \
		-print | xargs rm -f
	rm -rf $(TARGET) .depend

install:

distclean: clean

ifeq (.depend,$(wildcard .depend))
include .depend
endif

depend:
	$(CC)  -M  $(CFLAGS)   $(INCS) $(SRCS) > .depend
