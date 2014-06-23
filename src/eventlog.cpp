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
 * @version   0.5.0.1
 */

#include "eventlog.h"

#include <assert.h>
#include <stdio.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "config.h"

#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS
  typedef EventType Event;
#else
  typedef EventType Event;
#endif

typedef std::vector< Event > EventLog;

EventLog g_eventLog[LWM_MAX_THREADS][LWM_MAX_TX_TYPES];

typedef struct TxInfo_s
{
  unsigned long started;
  unsigned long committed;
  unsigned long aborted;
  unsigned long reads;
  unsigned long writes;
} TxInfo;

#if LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS
  void logEvent(thread_id_t tid, EventType type, tx_type_t txid)
  {
    g_eventLog[tid][txid].push_back(type);
  }
#endif

#if FALSE
void printStatistics()
{
  TxInfo* txInfo = NULL;
  unsigned long txStarted = 0;
  unsigned long txCommitted = 0;
  unsigned long txAborted = 0;
  unsigned long txReads = 0;
  unsigned long txWrites = 0;
  std::map< std::string, TxInfo > txInfoTable;

  for (int i = 0; i < LWM_MAX_THREADS; i++)
  {
    for (std::vector< Event >::iterator it = g_eventLog[i].events.begin();
      it != g_eventLog[i].events.end(); it++)
    {
      switch (it->type)
      {
        case TX_START:
          txInfo = &txInfoTable[std::string(it->file) + ":" + std::to_string(it->line)];
          ++txStarted;
          ++txInfo->started;
          break;
        case TX_COMMIT:
          assert(txInfo != NULL);
          ++txCommitted;
          ++txInfo->committed;
          break;
        case TX_ABORT:
          assert(txInfo != NULL);
          ++txAborted;
          ++txInfo->aborted;
          break;
        case TX_READ:
          assert(txInfo != NULL);
          ++txReads;
          ++txInfo->reads;
          break;
        case TX_WRITE:
          assert(txInfo != NULL);
          ++txWrites;
          ++txInfo->writes;
          break;
        default:
          assert(false);
          break;
      }
    }
  }

  std::cout << "Global Statistics\n";
  std::cout << "  Transactions started: " << txStarted << "\n";
  std::cout << "  Transactions committed: " << txCommitted << "\n";
  std::cout << "  Transactions aborted: " << txAborted << "\n";
  std::cout << "  Transactional reads: " << txReads << "\n";
  std::cout << "  Transactional writes: " << txWrites << "\n";
  std::cout << "Per-Transaction-Type Statistics\n";

  for (std::map< std::string, TxInfo >::iterator it = txInfoTable.begin();
    it != txInfoTable.end(); it++)
  {
    std::cout << "  Transaction " << it->first << "\n"
      << "    Transactions started: " << it->second.started << "\n"
      << "    Transactions committed: " << it->second.committed << "\n"
      << "    Transactions aborted: " << it->second.aborted << "\n"
      << "    Transactional reads: " << it->second.reads << "\n"
      << "    Transactional writes: " << it->second.writes << "\n";
  }
}
#endif

/** End of file eventlog.cpp **/
