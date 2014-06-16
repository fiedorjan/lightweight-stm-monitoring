#
# Makefile for compiling STAMP programs with lightweight monitoring support
#
# File:      Makefile
# Author:    Jan Fiedor (fiedorjan@centrum.cz)
# Date:      Created 2014-06-12
# Date:      Last Update 2014-06-16
# Version:   0.3.3
#

export TL2_HOME ?= $(shell pwd)/tl2-x86-0.9.6
export STAMP_HOME ?= $(shell pwd)/stamp-0.9.10
export STAMP_PROGRAMS = bayes genome intruder kmeans labyrinth ssca2 vacation yada

all: eventlog tl2 stamp

eventlog:
	g++ -std=c++0x -c src/eventlog.cpp
	ar cr libeventlog.a eventlog.o

tl2: tl2-patch tl2-compile tl2-restore

tl2-patch:
	mv -n $(TL2_HOME)/Makefile $(TL2_HOME)/Makefile.orig
	cp $(TL2_HOME)/Makefile.orig $(TL2_HOME)/Makefile
	sed -i -e 's|^CFLAGS.*|CFLAGS  := -g -Wall -Winline -O3 -I$(shell pwd)/src|' $(TL2_HOME)/Makefile
	mv -n $(TL2_HOME)/tl2.c $(TL2_HOME)/tl2.c.orig
	cp $(TL2_HOME)/tl2.c.orig $(TL2_HOME)/tl2.c
	patch -d $(TL2_HOME) -u -i $(shell pwd)/src/tl2.patch

tl2-compile:
	$(MAKE) -C $(TL2_HOME)

tl2-restore:
	mv $(TL2_HOME)/tl2.c.orig $(TL2_HOME)/tl2.c
	mv $(TL2_HOME)/Makefile.orig $(TL2_HOME)/Makefile

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

stamp-compile:
	for program in $(STAMP_PROGRAMS); do \
    $(MAKE) -C $(STAMP_HOME)/$$program -f Makefile.stm || ($(MAKE) stamp-restore && false); \
  done

stamp-restore:
	mv $(STAMP_HOME)/common/Defines.common.mk.orig $(STAMP_HOME)/common/Defines.common.mk
	mv $(STAMP_HOME)/common/Makefile.stm.orig $(STAMP_HOME)/common/Makefile.stm

clean:
	rm -rf *.a *.o
	$(MAKE) -C $(TL2_HOME) clean
	for program in $(STAMP_PROGRAMS); do $(MAKE) -C $(STAMP_HOME)/$$program -f Makefile.stm clean; done

# End of file Makefile
