/**
 * @brief Contains implementation of an event log.
 *
 * A file containing implementation of functions and data structures needed to
 *   use the event log.
 *
 * @file      eventlog.cpp
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-13
 * @date      Last Update 2014-06-23
 * @version   0.7
 */

#include "eventlog.h"

#include <assert.h>

#include <vector>

#include "config.h"
#include "stats.h"

#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS
  typedef EventType Event;
#elif LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS
  typedef struct Event_s
  {
    EventType type; //!< Event type.
    tx_type_t txid; //!< Transaction type identifier.

    Event_s(EventType evt, tx_type_t txt) : type(evt), txid(txt) {}
  } Event;
#else
  typedef EventType Event;
#endif

typedef std::vector< Event > EventLog;

#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS
  EventLog g_eventLog[LWM_MAX_THREADS][LWM_MAX_TX_TYPES];
#elif LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS
  EventLog g_eventLog[LWM_MAX_THREADS];
#endif

#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS || LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS
void logEvent(thread_id_t tid, EventType type, tx_type_t txid)
{
#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS
  g_eventLog[tid][txid].push_back(type);
#elif LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS
  g_eventLog[tid].push_back(Event(type, txid));
#endif
}

void printStats()
{
  Stats stats;

  for (int tid = 0; tid < LWM_MAX_THREADS; tid++)
  {
#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS
    for (int txid = 0; txid < LWM_MAX_TX_TYPES; txid++)
    {
      tx_op_counter_t starts = 0;
      tx_op_counter_t commits = 0;
      tx_op_counter_t aborts = 0;

      for (EventLog::iterator it = g_eventLog[tid][txid].begin();
        it != g_eventLog[tid][txid].end(); ++it)
      { // Count the TM operations
        switch (*it)
        { // Distinguish between different types of TM operations
          case TX_START:
            ++starts;
            break;
          case TX_COMMIT:
            ++commits;
            break;
          default:
            assert(false);
            break;
        }
      }

      stats.starts += starts;
      stats.commits += commits;
      stats.aborts += starts - commits;

      stats.txs[txid].starts += starts;
      stats.txs[txid].commits += commits;
      stats.txs[txid].aborts += starts - commits;

      stats.txs[txid].ptstarts.push_back(starts);
      stats.txs[txid].ptcommits.push_back(commits);
      stats.txs[txid].ptaborts.push_back(starts - commits);
    }
#elif LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS
    for (int txid = 0; txid < LWM_MAX_TX_TYPES; txid++)
    { // Initialise statistics for this thread
      stats.txs[txid].ptstarts.push_back(0);
      stats.txs[txid].ptcommits.push_back(0);
    }

    for (EventLog::iterator it = g_eventLog[tid].begin();
      it != g_eventLog[tid].end(); ++it)
    { // Process all events of this thread
      switch (it->type)
      { // Distinguish between different types of TM operations
        case TX_START:
          ++stats.starts;
          ++stats.txs[it->txid].starts;
          ++stats.txs[it->txid].ptstarts[tid];
          break;
        case TX_COMMIT:
          ++stats.commits;
          ++stats.txs[it->txid].commits;
          ++stats.txs[it->txid].ptcommits[tid];
          break;
        default:
          assert(false);
          break;
      }
    }

    for (int txid = 0; txid < LWM_MAX_TX_TYPES; txid++)
    { // Initialise statistics for this thread
      stats.aborts = stats.starts - stats.commits;
      stats.txs[txid].aborts = (stats.txs[txid].starts - stats.txs[txid].commits);
      stats.txs[txid].ptaborts.push_back(stats.txs[txid].ptstarts[tid] - stats.txs[txid].ptcommits[tid]);
    }
#endif
  }

  printGlobalStats(stats);
  printPerTxTypeStats(stats);
}
#endif

/** End of file eventlog.cpp **/
