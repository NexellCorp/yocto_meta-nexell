################################################################################
#   Build Options
################################################################################
OPTS		:= -Wall -O2 -Wextra -Wcast-align -Wno-unused-parameter \
			-Wshadow -Wwrite-strings -Wcast-qual -fno-strict-aliasing \
			-fstrict-overflow -fsigned-char -fno-omit-frame-pointer \
			-fno-optimize-sibling-calls

COPTS		:= $(OPTS)
CXXOPTS		:= $(OPTS) -Wnon-virtual-dtor

CFLAGS		+= $(OPTS)
CXXFLAGS	+= $(OPTS)
AFLAGS		+=

ARFLAGS		+= crv
LDFLAGS		+= -static

LIBRARY		:=

#########################################################################
# Toolchain.
#########################################################################
INCLUDE		+=
LIBRARY		+= -lstdc++

################################################################################
# Target
#########################################################################
CXXOBJS	:=
CXXOBJS	+= nx_init.o

CFLAGS	+= -g

TARGET	= nx_init

################################################################################
# Build
#########################################################################
OBJS	:= $(COBJS) $(CXXOBJS)

all: $(TARGET)

$(TARGET):	depend $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBRARY)

install:

clean:
	rm -rf $(TARGET) *.o .depend

distclean: clean

#########################################################################
# Dependency
ifeq (.depend,$(wildcard .depend))
include .depend
endif

SRCS := $(COBJS:.o=.c) $(AOBJS:.o=.s) $(CXXOBJS:.o=.cpp)
INCS := $(INCLUDE)
depend dep:
	$(CC) -M $(CFLAGS) $(AFLGAS) $(CXXFLAGS) $(INCS) $(SRCS) > .depend

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

%.o: %.s
	$(AS) $(AFLAGS) $(INCLUDE) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<
