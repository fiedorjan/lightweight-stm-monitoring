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

#define LOG_EVENT(type, tid, file, line) g_eventLog[tid].events.push_back(Event(type, file, line))

typedef enum EventType_e
{
  TX_START,
  TX_COMMIT,
  TX_ABORT,
  TX_READ,
  TX_WRITE
} EventType;

typedef struct Event_s
{
  EventType type;
  const char* file;
  unsigned long line;

  Event_s(EventType t, const char* f, unsigned long l) : type(t), file(f), line(l) {}
} Event;

typedef struct EventLog_s
{
  std::vector< Event > events;
} EventLog;

extern EventLog g_eventLog[EVENTLOG_THREADS];

void printStatistics();

#endif /* __EVENTLOG_H__ */

/** End of file eventlog.h **/
