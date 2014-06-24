/**
 * @brief Contains definitions of lightweight monitoring macros and functions.
 *
 * A file containing definitions of lightweight monitoring macros and functions.
 *
 * @file      lwm.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-19
 * @date      Last Update 2014-06-24
 * @version   0.3.4
 */

#ifndef __LWM_H__
  #define __LWM_H__

#include "config.h"
#include "defs.h"

#if LWM_TYPE == LWM_FAST_PER_TX_TYPE_ABORTS
  #include "statscollector.h"

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
#elif LWM_TYPE == LWM_EVT_LOG_PER_TX_TYPE_ABORTS || LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS
  #include "eventlog.h"

  #define STM_BEGIN(isReadOnly) \
    do { \
      STM_JMPBUF_T STM_JMPBUF; \
      int STM_RO_FLAG = isReadOnly; \
      sigsetjmp(STM_JMPBUF, 1); \
      TxStart(STM_SELF, &STM_JMPBUF, &STM_RO_FLAG); \
      LOG_EVENT(*(long*)STM_SELF, TX_START, __COUNTER__); \
    } while (0) /* enforce comma */

  #define STM_END() \
    TxCommit(STM_SELF); \
    LOG_EVENT(*(long*)STM_SELF, TX_COMMIT, __COUNTER__ - 1)

#if LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS
  #if LWM_TRACK_READS == 1
    // TODO
  #endif

  #if LWM_TRACK_WRITES == 1
    // TODO
  #endif
#endif

#endif

API_FUNCTION void printStats();

#define STM_SHUTDOWN() \
  TxShutdown(); \
  printStats()

#endif /* __LWM_H__ */

/** End of file lwm.h **/
