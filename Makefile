#
# Makefile for compiling STAMP programs with lightweight monitoring support
#
# File:      Makefile
# Author:    Jan Fiedor (fiedorjan@centrum.cz)
# Date:      Created 2014-06-12
# Date:      Last Update 2014-06-12
# Version:   0.1
#

export TL2_HOME ?= ./tl2-x86-0.9.6
export STAMP_HOME ?= ./stamp-0.9.10
export STAMP_PROGRAMS = bayes genome intruder kmeans labyrinth ssca2 vacation yada

all: tl2 stamp

tl2:
	make -C $(TL2_HOME)

stamp:
	cp $(STAMP_HOME)/common/Defines.common.mk $(STAMP_HOME)/common/Defines.common.mk.orig
	sed -i -e 's|^STM.*|STM := $(TL2_HOME)|' $(STAMP_HOME)/common/Defines.common.mk
	sed -i -e 's|^CFLAGS   += -I.*|CFLAGS   += -I$$(LIB) -I$(shell pwd)/src|' $(STAMP_HOME)/common/Defines.common.mk
	for program in $(STAMP_PROGRAMS); do make -C $(STAMP_HOME)/$$program -f Makefile.stm; done
	mv $(STAMP_HOME)/common/Defines.common.mk.orig $(STAMP_HOME)/common/Defines.common.mk

clean:
	make -C $(TL2_HOME) clean
	for program in $(STAMP_PROGRAMS); do make -C $(STAMP_HOME)/$$program -f Makefile.stm clean; done

# End of file Makefile
