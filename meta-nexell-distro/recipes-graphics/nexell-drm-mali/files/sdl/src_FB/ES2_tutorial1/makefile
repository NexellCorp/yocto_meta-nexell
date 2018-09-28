########################################################################
# Get Linux	Build Enviornment:
include	../build.env

######################################################################
# Build	options
INCLUDE	+= -I./
LIBRARY	+= -L../lib -lMali -lEGL -lGLESv2  -ldl
LIBRARY	+= -lstdc++	-lpthread
CPPFLAGS += "-D__int64=long long"
CFLAGS += "-D__int64=long long"
CFLAGS += "-mfloat-abi=hard"

######################################################################
# Target
COBJS	:=
CPPOBJS	:= tutorial1.o t1_render.o shaders.o Fake_OS.o
TARGET	:= tutorial1_es2


######################################################################
# Build
OBJS	:= $(COBJS)	$(CPPOBJS)

all: $(TARGET)

$(TARGET): depend $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $@	$(LIBRARY)

clean:
	@find $(OBJTREE) -type f \
		\( -name '*.o'	-o -name $(TARGET) -o -name	'*.map'	\) -print \
		| xargs	rm -f

#########################################################################
# Dependency
ifeq (dependency_list_$(TARGET),$(wildcard dependency_list_$(TARGET)))
include	dependency_list_$(TARGET)
endif

SRCS :=	$(COBJS:.o=.c) $(CPPOBJS:.o=.cpp)
INCS :=	$(INCLUDE)
depend dep:
	$(CC)  -M  $(CPPFLAGS) $(CFLAGS) $(INCS) $(INCLUDE)	$(SRCS)	> dependency_list_$(TARGET)



