#!/bin/bash
#
# Description:
#   A script simplifying building STAMP programs with the LWM support.
# Author:
#   Jan Fiedor
# Version:
#   0.1
# Created:
#   01.07.2014
# Last Update:
#   01.07.2014
#

# Settings section
# ----------------

# Path to the file containing the configuration of the lightweight monitoring
LWM_CONFIG="./src/config.h"

# Supported variations of the lightweight monitoring (from none to all Tx ops)
LWM_VARIATIONS=("orig" "sc" "el" "el-ts" "el-a" "el-a-ts" "el-arw" "el-arw-ts")

# A list of STAMP programs compiled when building the STAMP benchmark
STAMP_PROGRAMS=("bayes genome intruder kmeans labyrinth ssca2 vacation yada")

# Functions section
# -----------------

#
# Description:
#   Configures the lightweight monitoring.
# Parameters:
#   [STRING] A configuration of the lightweight monitoring. 
# Output:
#   None
# Return:
#   Nothing
#
configure()
{
  for item in $1; do
    local pair=(${item//=/ })
    sed -i -e "s/#define ${pair[0]}.*/#define ${pair[0]} ${pair[1]}/g" $LWM_CONFIG
  done
}

#
# Description:
#   Builds the STAMP benchmark.
# Parameters:
#   [STRING] A type of the LWM support. 
# Output:
#   Detailed information about the build process.
# Return:
#   Nothing
#
build()
{
  # Perform a clean build
  make clean

  # Save the current configuration
  mv $LWM_CONFIG $LWM_CONFIG.current
  cp $LWM_CONFIG.current $LWM_CONFIG

  # Build the STAMP benchmark
  case "$1" in
    "orig") # Original (without lightweight monitoring)
      make
      ;;
    "sc") # Stats collector
      configure "\
        LWM_TYPE=LWM_FAST_PER_TX_TYPE_ABORTS \
        LWM_COLLECT_TIMESTAMPS=0 \
        LWM_TRACK_ABORTS=0 \
        LWM_TRACK_READS=0 \
        LWM_TRACK_WRITES=0"
      make lwm
      ;;
    "el") # Event log
      configure "\
        LWM_TYPE=LWM_EVT_LOG_PER_THREAD_ABORTS \
        LWM_COLLECT_TIMESTAMPS=0 \
        LWM_TRACK_ABORTS=0 \
        LWM_TRACK_READS=0 \
        LWM_TRACK_WRITES=0"
      make lwm
      ;;
    "el-ts") # Event log (with timestamps)
      configure "\
        LWM_TYPE=LWM_EVT_LOG_PER_THREAD_ABORTS \
        LWM_COLLECT_TIMESTAMPS=1 \
        LWM_TRACK_ABORTS=0 \
        LWM_TRACK_READS=0 \
        LWM_TRACK_WRITES=0"
      make lwm
      ;;
    "el-a") # Event log with tracking of aborts
      configure "\
        LWM_TYPE=LWM_EVT_LOG_PER_THREAD_ABORTS \
        LWM_COLLECT_TIMESTAMPS=0 \
        LWM_TRACK_ABORTS=1 \
        LWM_TRACK_READS=0 \
        LWM_TRACK_WRITES=0"
      make lwm
      ;;
    "el-a-ts") # Event log with tracking of aborts (with timestamps)
      configure "\
        LWM_TYPE=LWM_EVT_LOG_PER_THREAD_ABORTS \
        LWM_COLLECT_TIMESTAMPS=1 \
        LWM_TRACK_ABORTS=1 \
        LWM_TRACK_READS=0 \
        LWM_TRACK_WRITES=0"
      make lwm
      ;;
    "el-arw") # Event log with tracking of aborts, reads and writes
      configure "\
        LWM_TYPE=LWM_EVT_LOG_PER_THREAD_ABORTS \
        LWM_COLLECT_TIMESTAMPS=0 \
        LWM_TRACK_ABORTS=1 \
        LWM_TRACK_READS=1 \
        LWM_TRACK_WRITES=1"
      make lwm
      ;;
    "el-arw-ts") # Event log with tracking of aborts, reads and writes (with timestamps)
      configure "\
        LWM_TYPE=LWM_EVT_LOG_PER_THREAD_ABORTS \
        LWM_COLLECT_TIMESTAMPS=1 \
        LWM_TRACK_ABORTS=1 \
        LWM_TRACK_READS=1 \
        LWM_TRACK_WRITES=1"
      make lwm
      ;;
  esac

  # Restore the current configuration
  mv -f $LWM_CONFIG.current $LWM_CONFIG

  # Flag the compiled program with the type of the LWM support
  for program in ${STAMP_PROGRAMS[@]}; do
    mv -f $STAMP_HOME/$program/$program $STAMP_HOME/$program/$program-$1
  done
}

# Program section
# ---------------

# Check where are the sources of the STAMP benchmark 
if [ -z "$STAMP_HOME" ]; then
  STAMP_HOME="./stamp-0.9.10"
fi

# Build STAMP benchmark
case "$1" in
  "lwm") # Build the STAMP with the current configuration
    make clean
    make lwm
    ;;
  "all") # Build the STAMP with all types of LWM supports
    for variation in ${LWM_VARIATIONS[@]}; do
      build $variation
    done
    ;;
  *) # Build the STAMP with a specific type of LWM support
    for variation in ${LWM_VARIATIONS[@]}; do
      if [ "$variation" == "$1" ]; then
        build $variation
      fi
    done
    ;;
esac

# End of script
