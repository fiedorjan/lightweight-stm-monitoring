/**
 * @brief Contains definition of an event log.
 *
 * A file containing definitions of functions and data structures needed to use
 *   the event log.
 *
 * @file      eventlog.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-13
 * @date      Last Update 2014-06-13
 * @version   0.1
 */

#ifndef __EVENTLOG_H__
  #define __EVENTLOG_H__

#include <vector>

#define EVENTLOG_THREADS 8

#define LOG_EVENT(tid, file, line) g_eventLog[tid].events.push_back(Event(file, line))

typedef struct Event_s
{
  const char* file;
  unsigned long line;

  Event_s(const char* f, unsigned long l) : file(f), line(l) {}
} Event;

typedef struct EventLog_s
{
  std::vector< Event > events;
} EventLog;

extern EventLog g_eventLog[EVENTLOG_THREADS];

void printEvents();

#endif /* __EVENTLOG_H__ */

/** End of file eventlog.h **/
