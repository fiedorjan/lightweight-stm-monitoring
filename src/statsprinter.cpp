/**
 * @brief Contains implementation of functions for printing statistics.
 *
 * A file containing implementation of functions for printing statistics.
 *
 * @file      stats.cpp
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-23
 * @date      Last Update 2014-06-23
 * @version   0.1
 */

#include "statsprinter.h"

#include <iostream>

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
    }
  }
}

/** End of file stats.cpp **/
