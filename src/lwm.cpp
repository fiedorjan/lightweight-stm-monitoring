/**
 * @brief Contains implementation of lightweight monitoring functions.
 *
 * A file containing implementation of lightweight monitoring functions.
 *
 * @file      lwm.cpp
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-19
 * @date      Last Update 2014-06-19
 * @version   0.2
 */

#include "lwm.h"

#include <iostream>
#include <vector>

TxInfo g_stats[MAXIMUM_THREADS][MAXIMUM_TRANSACTIONS];

typedef struct TxStats_s
{
  tx_op_counter_t started = 0;
  tx_op_counter_t committed = 0;
  tx_op_counter_t aborted = 0;
  std::vector< tx_op_counter_t > starts;
  std::vector< tx_op_counter_t > commits;
  std::vector< tx_op_counter_t > aborts;
} TxStats;

template < typename T >
inline
std::string printVector(std::vector< T >& v)
{
  std::string output;

  for (typename std::vector< T >::iterator it = v.begin(); it != v.end(); ++it)
  {
    if (it == v.begin())
      output += std::to_string(*it);
    else
      output += "," + std::to_string(*it);
  }

  return output;
}

void printStats()
{
  tx_op_counter_t starts = 0;
  tx_op_counter_t commits = 0;
  tx_op_counter_t aborts = 0;

  TxStats stats[MAXIMUM_TRANSACTIONS];

  for (int txid = 0; txid < MAXIMUM_TRANSACTIONS; txid++)
  {
    for (int tid = 0; tid < MAXIMUM_THREADS; tid++)
    {
      starts += g_stats[tid][txid].starts;
      commits += g_stats[tid][txid].commits;
      aborts += (g_stats[tid][txid].starts - g_stats[tid][txid].commits);

      stats[txid].started += g_stats[tid][txid].starts;
      stats[txid].committed += g_stats[tid][txid].commits;
      stats[txid].aborted += (g_stats[tid][txid].starts - g_stats[tid][txid].commits);

      stats[txid].starts.push_back(g_stats[tid][txid].starts);
      stats[txid].commits.push_back(g_stats[tid][txid].commits);
      stats[txid].aborts.push_back(g_stats[tid][txid].starts - g_stats[tid][txid].commits);
    }
  }

  std::cout << "Global Statistics\n";
  std::cout << "  Starts: " << starts << "\n";
  std::cout << "  Commits: " << commits << "\n";
  std::cout << "  Aborts: " << aborts << "\n";
  std::cout << "Per-Transaction-Type Statistics\n";

  for (int txid = 0; txid < MAXIMUM_TRANSACTIONS; txid++)
  {
    if (stats[txid].started > 0)
    { // Filter out non-existent transactions for which we allocated space
      std::cout << "  Transaction " << txid << "\n";
      std::cout << "    Starts: " << stats[txid].started << " ["
        << printVector(stats[txid].starts) << "]\n";
      std::cout << "    Commits: " << stats[txid].committed << " ["
        << printVector(stats[txid].commits) << "]\n";
      std::cout << "    Aborts: " << stats[txid].aborted << " ["
        << printVector(stats[txid].aborts) << "]\n";
    }
  }
}

/** End of file lwm.cpp **/
