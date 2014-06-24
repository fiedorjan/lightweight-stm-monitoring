/**
 * @brief Contains definition and implementation of various wrappers.
 *
 * A file containing definition and implementation of various wrappers.
 *
 * @file      wrappers.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-24
 * @date      Last Update 2014-06-24
 * @version   0.1
 */

#ifndef __WRAPPERS_H__
  #define __WRAPPERS_H__

#include "config.h"
#include "eventlog.h"

#if LWM_TRACK_READS == 1
static inline
intptr_t TxReadWrapper(Thread* Self, volatile intptr_t* Addr)
{
  LOG_EVENT(*(long*)Self, TX_READ, -1);
  return TxLoad(Self, Addr);
}
#endif

#if LWM_TRACK_WRITES == 1
static inline
void TxWriteWrapper(Thread* Self, volatile intptr_t* addr, intptr_t valu)
{
  LOG_EVENT(*(long*)Self, TX_WRITE, -1);
  TxStore(Self, addr, valu);
}
#endif

#endif /* __WRAPPERS_H__ */

/** End of file wrappers.h **/
