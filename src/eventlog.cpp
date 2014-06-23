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
 * @version   0.6
 */

#include "eventlog.h"

#include <assert.h>

#include <vector>

#include "config.h"
#include "stats.h"

#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS
  typedef EventType Event;
#else
  typedef EventType Event;
#endif

typedef std::vector< Event > EventLog;

EventLog g_eventLog[LWM_MAX_THREADS][LWM_MAX_TX_TYPES];

#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS
void logEvent(thread_id_t tid, EventType type, tx_type_t txid)
{
  g_eventLog[tid][txid].push_back(type);
}

void printStats()
{
  Stats stats;

  for (int txid = 0; txid < LWM_MAX_TX_TYPES; txid++)
  {
    for (int tid = 0; tid < LWM_MAX_THREADS; tid++)
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
  }

  printGlobalStats(stats);
  printPerTxTypeStats(stats);
}
#endif

/** End of file eventlog.cpp **/
