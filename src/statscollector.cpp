/**
 * @brief Contains implementation of functions for collecting statistics.
 *
 * A file containing implementation of functions for collecting statistics.
 *
 * @file      statscollector.cpp
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-19
 * @date      Last Update 2014-06-24
 * @version   0.3.1
 */

#include "statscollector.h"

#include "lwm.h"
#include "statsprinter.h"

#if LWM_TYPE == LWM_FAST_PER_TX_TYPE_ABORTS
TxInfo g_stats[LWM_MAX_THREADS][LWM_MAX_TX_TYPES];

void printStats()
{
  Stats stats;

  for (int txid = 0; txid < LWM_MAX_TX_TYPES; txid++)
  {
    for (int tid = 0; tid < LWM_MAX_THREADS; tid++)
    {
      stats.starts += g_stats[tid][txid].starts;
      stats.commits += g_stats[tid][txid].commits;
      stats.aborts += (g_stats[tid][txid].starts - g_stats[tid][txid].commits);

      stats.txs[txid].starts += g_stats[tid][txid].starts;
      stats.txs[txid].commits += g_stats[tid][txid].commits;
      stats.txs[txid].aborts += (g_stats[tid][txid].starts - g_stats[tid][txid].commits);

      stats.txs[txid].ptstarts.push_back(g_stats[tid][txid].starts);
      stats.txs[txid].ptcommits.push_back(g_stats[tid][txid].commits);
      stats.txs[txid].ptaborts.push_back(g_stats[tid][txid].starts - g_stats[tid][txid].commits);
    }
  }

  printGlobalStats(stats);
  printPerTxTypeStats(stats);
}
#endif

/** End of file statscollector.cpp **/
