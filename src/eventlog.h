/**
 * @brief Contains definition of an event log.
 *
 * A file containing definitions of functions and data structures needed to use
 *   the event log.
 *
 * @file      eventlog.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-13
 * @date      Last Update 2014-06-24
 * @version   0.3.4
 */

#ifndef __EVENTLOG_H__
  #define __EVENTLOG_H__

#include "config.h"
#include "defs.h"

#if LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS && LWM_COLLECT_TIMESTAMPS == 1
  #include "rdtsc.h"
#endif

typedef enum EventType_e
{
  TX_START,
  TX_COMMIT,
  TX_ABORT,
  TX_READ,
  TX_WRITE
} EventType;

#if LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS && LWM_COLLECT_TIMESTAMPS == 1
  API_FUNCTION void logEvent(thread_id_t tid, EventType type, tx_type_t txid, timestamp_t ts);

  #define LOG_EVENT(tid, type, txid) logEvent(tid, type, txid, rdtsc().time_stamp)
#elif LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS || LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS
  API_FUNCTION void logEvent(thread_id_t tid, EventType type, tx_type_t txid);

  #define LOG_EVENT(tid, type, txid) logEvent(tid, type, txid)
#endif

#endif /* __EVENTLOG_H__ */

/** End of file eventlog.h **/
