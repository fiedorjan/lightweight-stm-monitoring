/**
 * @brief Contains implementation of lightweight monitoring functions.
 *
 * A file containing implementation of lightweight monitoring functions.
 *
 * @file      lwm.cpp
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-19
 * @date      Last Update 2014-06-19
 * @version   0.1
 */

#include "lwm.h"

#include <iostream>

TxInfo g_stats[MAXIMUM_THREADS][MAXIMUM_TRANSACTIONS];

void printStats()
{
  for (int txid = 0; txid < MAXIMUM_TRANSACTIONS; txid++)
  {
    std::cout << "  Transaction " << txid << "\n";

    std::cout << "    Starts:";

    for (int tid = 0; tid < MAXIMUM_THREADS; tid++)
    {
       std::cout << " " << g_stats[tid][txid].starts;
    }

    std::cout << "\n";
  }
}

/** End of file lwm.cpp **/
