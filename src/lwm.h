/**
 * @brief Contains definitions of lightweight monitoring macros and functions.
 *
 * A file containing definitions of lightweight monitoring macros and functions.
 *
 * @file      lwm.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-19
 * @date      Last Update 2014-06-20
 * @version   0.2.1
 */

#ifndef __LWM_H__
  #define __LWM_H__

#include "config.h"
#include "defs.h"

#if LWM_TYPE == LWM_FAST_PER_TX_TYPE_ABORTS
  typedef struct TxInfo_s
  {
    tx_op_counter_t starts;
    tx_op_counter_t commits;
  } TxInfo;

  extern TxInfo g_stats[LWM_MAX_THREADS][LWM_MAX_TX_TYPES];

  #ifdef __cplusplus
    extern "C" {
  #endif

  void printStats();

  #ifdef __cplusplus
    }
  #endif

  #define STM_BEGIN(isReadOnly) \
    do { \
      STM_JMPBUF_T STM_JMPBUF; \
      int STM_RO_FLAG = isReadOnly; \
      sigsetjmp(STM_JMPBUF, 1); \
      TxStart(STM_SELF, &STM_JMPBUF, &STM_RO_FLAG); \
      ++g_stats[*(long*)STM_SELF][__COUNTER__].starts; \
    } while (0) /* enforce comma */

  #define STM_END() \
    TxCommit(STM_SELF); \
    ++g_stats[*(long*)STM_SELF][__COUNTER__ - 1].commits

  #define STM_SHUTDOWN() \
    TxShutdown(); \
    printStats()
#endif

#endif /* __LWM_H__ */

/** End of file lwm.h **/
