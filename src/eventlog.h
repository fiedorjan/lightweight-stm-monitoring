/**
 * @brief Contains definition of an event log.
 *
 * A file containing definitions of functions and data structures needed to use
 *   the event log.
 *
 * @file      eventlog.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-13
 * @date      Last Update 2014-06-16
 * @version   0.2
 */

#ifndef __EVENTLOG_H__
  #define __EVENTLOG_H__

#define EVENTLOG_THREADS 8

#define LOG_EVENT(type, tid, file, line) logEvent(tid, type, file, line)

typedef enum EventType_e
{
  TX_START,
  TX_COMMIT,
  TX_ABORT,
  TX_READ,
  TX_WRITE
} EventType;

#ifdef __cplusplus
extern "C" {
#endif

void logEvent(long tid, EventType type, const char* file, unsigned long line);
void printStatistics();

#ifdef __cplusplus
}
#endif

#endif /* __EVENTLOG_H__ */

/** End of file eventlog.h **/
