/**
 * @brief Contains definitions of lightweight monitoring macros and functions.
 *
 * A file containing definitions of lightweight monitoring macros and functions.
 *
 * @file      lwm.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-19
 * @date      Last Update 2014-06-24
 * @version   0.4
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

#if LWM_TYPE == LWM_EVT_LOG_PER_THREAD_ABORTS && (LWM_TRACK_READS == 1 || LWM_TRACK_WRITES == 1)
  #include "wrappers.h"

  #if LWM_TRACK_READS == 1
    #define STM_READ(var) TxReadWrapper(STM_SELF, (vintp*)(void*)&(var))
    #define STM_READ_F(var) IP2F(TxReadWrapper(STM_SELF, (vintp*)FP2IPP(&(var))))
    #define STM_READ_P(var) IP2VP(TxReadWrapper(STM_SELF, (vintp*)(void*)&(var)))
  #endif

  #if LWM_TRACK_WRITES == 1
    #define STM_WRITE(var, val) TxWriteWrapper(STM_SELF, (vintp*)(void*)&(var), (intptr_t)(val))
    #define STM_WRITE_F(var, val) TxWriteWrapper(STM_SELF, (vintp*)FP2IPP(&(var)), F2IP(val))
    #define STM_WRITE_P(var, val) TxWriteWrapper(STM_SELF, (vintp*)(void*)&(var), VP2IP(val))
  #endif
#endif

#endif

#define STM_SHUTDOWN() \
  TxShutdown(); \
  printStats()

#endif /* __LWM_H__ */

/** End of file lwm.h **/
