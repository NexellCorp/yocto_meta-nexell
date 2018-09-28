######################################################################
# Build options
export TOPDIR	:=$(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi | sed s,.*users,/home/users,)

CPPFLAGS = -Wall

INCLUDES ?= -I$(TOPDIR) \

#LIBS += -lkms -ldrm -ldrm_nexell -lnx_renderer -lnx_drm_allocator
LIBS += -lEGL -lGLESv2 -lSDL2
LIBS += -lrt -lpthread -lc -lstdc++
CPPFLAGS += -O3 $(INCLUDES) $(LDFLAGS) $(LIBS) -DGL_GLEXT_PROTOTYPES -DVR_DEBUG -std=c99 -Dlinux
CFLAGS += -O3 $(INCLUDES) $(LDFLAGS) $(LIBS) -DGL_GLEXT_PROTOTYPES -DVR_DEBUG -std=c99 -Dlinux

######################################################################
OBJS := \
	$(TOPDIR)/hello_world.o

SRCS = $(OBJS:.o=.cpp) $(OBJS:.o=.c)

TARGET = sdl_helloWorld

$(TARGET): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS)
#	$(CXX) -o $@ $(CPPFLAGS) $(INCLUDES) $(OBJS) $(LIBS)

all: $(TARGET)

.PHONY: clean

clean :
	rm -rf $(OBJS) $(TARGET)

