######################################################################
# Build options
export TOPDIR	:=$(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi | sed s,.*users,/home/users,)

#PLATFORM_LIB_PATH = ./export/lib/platform/so/drm
MALI_LIB_PATH = $(TOPDIR)/../example_import/lib

CPPFLAGS = -Wall

INCLUDES ?= -I$(TOPDIR) \
	-I$(TOPDIR)/../example_import/3rdparty/include/khronos \
	-I$(TOPDIR)/../example_import/include \
	-I$(TOPDIR)/../example_import/include/EGL/platform_drm

LDFLAGS ?= \
	-L$(MALI_LIB_PATH) \
	-L/$(SDKTARGETSYSROOT)/usr/lib \
	-L/$(SDKTARGETSYSROOT)/lib

LIBS += -lkms -ldrm -ldrm_nexell -lnx_renderer -lnx_drm_allocator
LIBS += -lEGL -lGLESv2 -lMali
LIBS += -lrt -lpthread -lc -lstdc++
CPPFLAGS += -O3 $(INCLUDES) $(LDFLAGS) $(LIBS) -DGL_GLEXT_PROTOTYPES -DVR_DEBUG -std=c99 -Dlinux
CFLAGS += -O3 $(INCLUDES) $(LDFLAGS) $(LIBS) -DGL_GLEXT_PROTOTYPES -DVR_DEBUG -std=c99 -Dlinux

######################################################################
OBJS := \
	$(TOPDIR)/tutorial1.o \
	$(TOPDIR)/t1_render.o \
	$(TOPDIR)/shaders.o \
	$(TOPDIR)/Fake_OS.o \
	$(TOPDIR)/__door128_tga.o \
	$(TOPDIR)/__fire128_tga.o

SRCS = $(OBJS:.o=.cpp) $(OBJS:.o=.c)

TARGET = tutorial1_es2_gbm

$(TARGET): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS)
#	$(CXX) -o $@ $(CPPFLAGS) $(INCLUDES) $(OBJS) $(LIBS)

all: $(TARGET)

.PHONY: clean

clean :
	rm -rf $(OBJS) $(TARGET)

