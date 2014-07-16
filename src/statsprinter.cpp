/**
 * @brief Contains implementation of functions for printing statistics.
 *
 * A file containing implementation of functions for printing statistics.
 *
 * @file      stats.cpp
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-23
 * @date      Last Update 2014-07-16
 * @version   0.3.1.1
 */

#include "statsprinter.h"

#include <iostream>
#include <string>

template < typename T >
inline
std::string toString(const std::vector< T >& v)
{
  std::string output;

  for (typename std::vector< T >::const_iterator it = v.begin(); it != v.end();
    ++it)
  {
    if (it == v.begin())
      output += std::to_string(*it);
    else
      output += "," + std::to_string(*it);
  }

  return output;
}

void printGlobalStats(const Stats& stats)
{
  std::cout << "Global Statistics\n";
  std::cout << "  Starts: " << stats.starts << "\n";
  std::cout << "  Commits: " << stats.commits << "\n";
  std::cout << "  Aborts: " << stats.aborts << "\n";
  std::cout << "  Reads: " << stats.reads << "\n";
  std::cout << "  Writes: " << stats.writes << "\n";
  std::cout << "  Average length of transactions: " << stats.avglength
    << " time units\n";
  std::cout << "  Amount of time spent in transactions: "
    << ((stats.utilization * 100) / stats.time) << "% ("
    << stats.utilization << " time units out of " << stats.time
    << ")\n";
}

void printPerTxTypeStats(const Stats& stats)
{
  std::cout << "Per-Transaction-Type Statistics\n";

  for (int txid = 0; txid < LWM_MAX_TX_TYPES; txid++)
  {
    if (stats.txs[txid].starts > 0)
    { // Filter out non-existent transactions for which we allocated space
      std::cout << "  Transaction " << txid << "\n";
      std::cout << "    Starts: " << stats.txs[txid].starts << " ["
        << toString(stats.txs[txid].ptstarts) << "]\n";
      std::cout << "    Commits: " << stats.txs[txid].commits << " ["
        << toString(stats.txs[txid].ptcommits) << "]\n";
      std::cout << "    Aborts: " << stats.txs[txid].aborts << " ["
        << toString(stats.txs[txid].ptaborts) << "]\n";
      std::cout << "    Reads: " << stats.txs[txid].reads << " ["
        << toString(stats.txs[txid].ptreads) << "]\n";
      std::cout << "    Writes: " << stats.txs[txid].writes << " ["
        << toString(stats.txs[txid].ptwrites) << "]\n";
      std::cout << "    Average length of transactions: "
        << stats.txs[txid].avglength << " time units\n";
      std::cout << "    Amount of time spent in transactions: "
        << ((stats.txs[txid].utilization * 100) / stats.time) << "% ("
        << stats.txs[txid].utilization << " time units out of " << stats.time
        << ")\n";
    }
  }
}

/** End of file stats.cpp **/
