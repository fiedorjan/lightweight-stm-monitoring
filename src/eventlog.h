/**
 * @brief Contains definition of an event log.
 *
 * A file containing definitions of functions and data structures needed to use
 *   the event log.
 *
 * @file      eventlog.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-13
 * @date      Last Update 2014-06-23
 * @version   0.3
 */

#ifndef __EVENTLOG_H__
  #define __EVENTLOG_H__

#include "defs.h"

typedef enum EventType_e
{
  TX_START,
  TX_COMMIT,
  TX_ABORT,
  TX_READ,
  TX_WRITE
} EventType;

#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS
  API_FUNCTION void logEvent(thread_id_t tid, EventType type, tx_type_t txid);

  #define LOG_EVENT(tid, type, txid) logEvent(tid, type, txid)
#endif

#endif /* __EVENTLOG_H__ */

/** End of file eventlog.h **/
