/**
 * @brief Contains definition of functions for collecting statistics.
 *
 * A file containing definition of functions for collecting statistics.
 *
 * @file      statscollector.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-24
 * @date      Last Update 2014-06-24
 * @version   0.1
 */

#ifndef __STATSCOLLECTOR_H__
  #define __STATSCOLLECTOR_H__

#include "defs.h"
#include "config.h"

typedef struct TxInfo_s
{
  tx_op_counter_t starts;
  tx_op_counter_t commits;
} TxInfo;

extern TxInfo g_stats[LWM_MAX_THREADS][LWM_MAX_TX_TYPES];

#endif /* __STATSCOLLECTOR_H__ */

/** End of file statscollector.h **/
