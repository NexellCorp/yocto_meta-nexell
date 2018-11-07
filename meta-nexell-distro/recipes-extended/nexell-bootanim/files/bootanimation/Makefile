################################################################################
#   Build Options
################################################################################
OPTS		:= -Wall -O3 -Wextra -Wcast-align -Wno-unused-parameter \
				-Wshadow -Wwrite-strings -Wcast-qual -fno-strict-aliasing \
				-fstrict-overflow -fsigned-char -fno-omit-frame-pointer \
				-fno-optimize-sibling-calls

COPTS		:= -Wno-implicit-function-declaration
CXXOPTS		:= -Wnon-virtual-dtor

CFLAGS		+= $(COPTS)
CXXFLAGS	+= $(CXXOPTS)
AFLAGS		+=

ARFLAGS		+= crv
LDFLAGS		+= -static

LIBRARY		:=

#########################################################################
#   Toolchain.
#########################################################################
INCLUDE		+=
LIBRARY		+= -lpng -lz -lm

################################################################################
# Target
COBJS	:=
COBJS	+= bootanimation.o fb.o jz.o png.o bmp.o

CFLAGS	+= -rdynamic

TARGET	= bootanimation

################################################################################
# Build
OBJS	:= $(COBJS)

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
	$(CC) -M $(CFLAGS) $(AFLGAS) $(INCS) $(SRCS) > .depend

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

%.o: %.s
	$(AS) $(AFLAGS) $(INCLUDE) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<
