#
# Makefile for compiling STAMP programs with lightweight monitoring support
#
# File:      Makefile
# Author:    Jan Fiedor (fiedorjan@centrum.cz)
# Date:      Created 2014-06-12
# Date:      Last Update 2014-06-24
# Version:   0.4.5
#

# Settings
# --------

# Default values of user-defined variables
export TL2_HOME ?= $(shell pwd)/tl2-x86-0.9.6
export STAMP_HOME ?= $(shell pwd)/stamp-0.9.10
export STAMP_PROGRAMS ?= bayes genome intruder kmeans labyrinth ssca2 vacation yada

# Internal variables
CPP = g++
CPPFLAGS = 
LWM_LIB_NAME = lwm
LWM_CPPFLAGS = $(CPPFLAGS) -std=c++0x

# Recipes
# -------

# Compiles TL2 library and STAMP benchmark with no modifications
all: tl2 stamp

# Compiles TL2 library and STAMP benchmark with lightweight monitoring support
lwm: lwm-lib tl2-lwm stamp-lwm

# Lightweight monitoring library
lwm-lib:
	$(CPP) $(LWM_CPPFLAGS) -c src/eventlog.cpp
	$(CPP) $(LWM_CPPFLAGS) -c src/statscollector.cpp
	$(CPP) $(LWM_CPPFLAGS) -c src/statsprinter.cpp
	ar cr lib$(LWM_LIB_NAME).a eventlog.o statscollector.o statsprinter.o

# TL2 library with no modifications
tl2: tl2-compile

# TL2 library with lightweight monitoring support
tl2-lwm: tl2-patch tl2-compile tl2-restore

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

# STAMP benchmark with no modifications
stamp: stamp-setup stamp-compile stamp-cleanup

# STAMP benchmark with lightweight monitoring support
stamp-lwm: stamp-patch stamp-compile stamp-restore

stamp-setup:
	mv -n $(STAMP_HOME)/common/Defines.common.mk $(STAMP_HOME)/common/Defines.common.mk.orig
	cp $(STAMP_HOME)/common/Defines.common.mk.orig $(STAMP_HOME)/common/Defines.common.mk
	sed -i -e 's|^STM.*|STM := $(TL2_HOME)|' $(STAMP_HOME)/common/Defines.common.mk

stamp-patch: stamp-setup
	sed -i -e 's|^CFLAGS   += -I.*|CFLAGS   += -I$$(LIB) -I$(shell pwd)/src|' $(STAMP_HOME)/common/Defines.common.mk
	mv -n $(STAMP_HOME)/common/Makefile.stm $(STAMP_HOME)/common/Makefile.stm.orig
	cp $(STAMP_HOME)/common/Makefile.stm.orig $(STAMP_HOME)/common/Makefile.stm
	sed -i -e 's|^LIBS.*|LIBS     += -ltl2 -l$(LWM_LIB_NAME)|' $(STAMP_HOME)/common/Makefile.stm
	sed -i -e 's|^LDFLAGS.*|LDFLAGS  += -L$$(STM) -L$(shell pwd)|' $(STAMP_HOME)/common/Makefile.stm

stamp-compile:
	for program in $(STAMP_PROGRAMS); do \
    $(MAKE) -C $(STAMP_HOME)/$$program -f Makefile.stm || ($(MAKE) stamp-restore && false); \
  done

stamp-cleanup:
	mv $(STAMP_HOME)/common/Defines.common.mk.orig $(STAMP_HOME)/common/Defines.common.mk

stamp-restore: stamp-cleanup
	mv $(STAMP_HOME)/common/Makefile.stm.orig $(STAMP_HOME)/common/Makefile.stm

clean:
	rm -rf *.a *.o
	$(MAKE) -C $(TL2_HOME) clean
	for program in $(STAMP_PROGRAMS); do $(MAKE) -C $(STAMP_HOME)/$$program -f Makefile.stm clean; done

# End of file Makefile
