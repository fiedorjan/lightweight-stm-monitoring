#
# Makefile for compiling STAMP programs with lightweight monitoring support
#
# File:      Makefile
# Author:    Jan Fiedor (fiedorjan@centrum.cz)
# Date:      Created 2014-06-12
# Date:      Last Update 2014-06-16
# Version:   0.2.1
#

export TL2_HOME ?= ./tl2-x86-0.9.6
export STAMP_HOME ?= ./stamp-0.9.10
export STAMP_PROGRAMS = bayes genome intruder kmeans labyrinth ssca2 vacation yada

all: eventlog tl2 stamp

eventlog:
	g++ -std=c++0x -c src/eventlog.cpp
	ar cr libeventlog.a eventlog.o

tl2:
	$(MAKE) -C $(TL2_HOME)

stamp: stamp-patch stamp-compile stamp-restore

stamp-patch:
	mv -n $(STAMP_HOME)/common/Defines.common.mk $(STAMP_HOME)/common/Defines.common.mk.orig
	cp $(STAMP_HOME)/common/Defines.common.mk.orig $(STAMP_HOME)/common/Defines.common.mk
	sed -i -e 's|^STM.*|STM := $(TL2_HOME)|' $(STAMP_HOME)/common/Defines.common.mk
	sed -i -e 's|^CFLAGS   += -I.*|CFLAGS   += -I$$(LIB) -I$(shell pwd)/src|' $(STAMP_HOME)/common/Defines.common.mk
	mv -n $(STAMP_HOME)/common/Makefile.stm $(STAMP_HOME)/common/Makefile.stm.orig
	cp $(STAMP_HOME)/common/Makefile.stm.orig $(STAMP_HOME)/common/Makefile.stm
	sed -i -e 's|^LIBS.*|LIBS     += -ltl2 -leventlog|' $(STAMP_HOME)/common/Makefile.stm
	sed -i -e 's|^LDFLAGS.*|LDFLAGS  += -L$$(STM) -L$(shell pwd)|' $(STAMP_HOME)/common/Makefile.stm
	mv -n $(STAMP_HOME)/common/Makefile.common $(STAMP_HOME)/common/Makefile.common.orig
	cp $(STAMP_HOME)/common/Makefile.common.orig $(STAMP_HOME)/common/Makefile.common
	sed -i -e 's|CC|CPP|' $(STAMP_HOME)/common/Makefile.common
	sed -i -e 's|CFLAGS|CPPFLAGS|' $(STAMP_HOME)/common/Makefile.common

stamp-compile:
	for program in $(STAMP_PROGRAMS); do \
    $(MAKE) -C $(STAMP_HOME)/$$program -f Makefile.stm || ($(MAKE) stamp-restore && false); \
  done

stamp-restore:
	mv $(STAMP_HOME)/common/Defines.common.mk.orig $(STAMP_HOME)/common/Defines.common.mk
	mv $(STAMP_HOME)/common/Makefile.stm.orig $(STAMP_HOME)/common/Makefile.stm
	mv $(STAMP_HOME)/common/Makefile.common.orig $(STAMP_HOME)/common/Makefile.common

clean:
	rm -rf *.a *.o
	$(MAKE) -C $(TL2_HOME) clean
	for program in $(STAMP_PROGRAMS); do $(MAKE) -C $(STAMP_HOME)/$$program -f Makefile.stm clean; done

# End of file Makefile
