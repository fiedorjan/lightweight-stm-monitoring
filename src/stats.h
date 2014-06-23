/**
 * @brief Contains definition of functions for printing statistics.
 *
 * A file containing definition of functions for printing statistics.
 *
 * @file      stats.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-23
 * @date      Last Update 2014-06-23
 * @version   0.1
 */

#ifndef __STATS_H__
  #define __STATS_H__

#include <vector>

#include "config.h"
#include "defs.h"

/**
 * @brief A structure containing per-transaction-type statistics.
 */
typedef struct TxStats_s
{
  tx_op_counter_t starts = 0; //!< Number of started transactions.
  tx_op_counter_t commits = 0; //!< Number of committed transactions.
  tx_op_counter_t aborts = 0; //!< Number of aborted transactions.
  /**
   * @brief Number of started transactions (per-thread).
   */
  std::vector< tx_op_counter_t > ptstarts;
  /**
   * @brief Number of committed transactions (per-thread).
   */
  std::vector< tx_op_counter_t > ptcommits;
  /**
   * @brief Number of aborted transactions (per-thread).
   */
  std::vector< tx_op_counter_t > ptaborts;
} TxStats;

/**
 * @brief A structure containing all available statistics.
 */
typedef struct Stats_s
{
  tx_op_counter_t starts = 0; //!< Number of started transactions.
  tx_op_counter_t commits = 0; //!< Number of committed transactions.
  tx_op_counter_t aborts = 0; //!< Number of aborted transactions.
  TxStats txs[LWM_MAX_TX_TYPES]; //!< Statistics for each transaction type.
} Stats;

void printGlobalStats(const Stats& stats);
void printPerTxTypeStats(const Stats& stats);

#endif /* __STATS_H__ */

/** End of file stats.h **/
