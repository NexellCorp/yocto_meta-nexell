DIR :=
DIR += src

CROSS_COMPILE ?= arm-linux-gnueabihf-

all:
	@for dir in $(DIR); do	\
	make CROSS_COMPILE=$(CROSS_COMPILE) -C $$dir || exit $?;	\
	done

clean:
	@for dir in $(DIR); do	\
	make -C $$dir clean || exit $?;	\
	done
