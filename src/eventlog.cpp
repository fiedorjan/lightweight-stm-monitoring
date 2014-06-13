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

#include <assert.h>
#include <stdio.h>

#include <iostream>
#include <map>
#include <string>

EventLog g_eventLog[EVENTLOG_THREADS];

typedef struct TxInfo_s
{
  unsigned long started;
} TxInfo;

void printStatistics()
{
  long txStarted = 0;
  std::map< std::string, TxInfo > txInfoTable;

  for (int i = 0; i < EVENTLOG_THREADS; i++)
  {
    for (std::vector< Event >::iterator it = g_eventLog[i].events.begin();
      it != g_eventLog[i].events.end(); it++)
    {
      TxInfo& txInfo = txInfoTable[std::string(it->file) + ":" + std::to_string(it->line)];

      switch (it->type)
      {
        case TX_START:
          ++txStarted;
          ++txInfo.started;
          break;
        default:
          assert(false);
          break;
      }
    }
  }

  std::cout << "Global Statistics\n";
  std::cout << "  Transactions started: " << txStarted << "\n";
  std::cout << "Per-Transaction-Type Statistics\n";

  for (std::map< std::string, TxInfo >::iterator it = txInfoTable.begin();
    it != txInfoTable.end(); it++)
  {
    std::cout << "  Transaction " << it->first << "\n"
      << "    Transactions started: " << it->second.started << "\n";
  }
}

/** End of file eventlog.cpp **/
