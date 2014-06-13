/**
 * @brief Contains implementation of an event log.
 *
 * A file containing implementation of functions and data structures needed to
 *   use the event log.
 *
 * @file      eventlog.cpp
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-13
 * @date      Last Update 2014-06-13
 * @version   0.1
 */

#include "eventlog.h"

#include <stdio.h>

EventLog g_eventLog[EVENTLOG_THREADS];

void printEvents()
{
  for (std::vector< Event >::iterator it = g_eventLog[0].events.begin();
    it != g_eventLog[0].events.end(); it++)
  {
    printf("%s, %d\n", it->file, it->line);
  }
}

/** End of file eventlog.cpp **/
