/* =============================================================================
 *
 * stm.h
 *
 * User program interface for STM. For an STM to interface with STAMP, it needs
 * to have its own stm.h for which it redefines the macros appropriately.
 *
 * =============================================================================
 *
 * Author: Chi Cao Minh
 *
 * =============================================================================
 */


#ifndef STM_H
#define STM_H 1


#include "tl2.h"
#include "util.h"
#include "eventlog.h"

#define STM_THREAD_T                    Thread
#define STM_SELF                        Self
#define STM_RO_FLAG                     ROFlag

#define STM_MALLOC(size)                TxAlloc(STM_SELF, size)
#define STM_FREE(ptr)                   TxFree(STM_SELF, ptr)


#  define malloc(size)                  tmalloc_reserve(size)
#  define calloc(n, size)               ({ \
                                            size_t numByte = (n) * (size); \
                                            void* ptr = tmalloc_reserve(numByte); \
                                            if (ptr) { \
                                                memset(ptr, 0, numByte); \
                                            } \
                                            ptr; \
                                        })
#  define realloc(ptr, size)            tmalloc_reserveAgain(ptr, size)
#  define free(ptr)                     tmalloc_release(ptr)

#  include <setjmp.h>
#  define STM_JMPBUF_T                  sigjmp_buf
#  define STM_JMPBUF                    buf


#define STM_VALID()                     (1)
#define STM_RESTART()                   do { \
                                            LOG_EVENT(TX_ABORT, *(long*)STM_SELF, __FILE__, __LINE__); \
                                            TxAbort(STM_SELF); \
                                        } while (0) /* enforce comma */

#define STM_STARTUP()                   TxOnce()
#define STM_SHUTDOWN()                  TxShutdown(); printStatistics()

#define STM_NEW_THREAD()                TxNewThread()
#define STM_INIT_THREAD(t, id)          TxInitThread(t, id)
#define STM_FREE_THREAD(t)              TxFreeThread(t)








#  define STM_BEGIN(isReadOnly)         do { \
                                            STM_JMPBUF_T STM_JMPBUF; \
                                            int STM_RO_FLAG = isReadOnly; \
                                            sigsetjmp(STM_JMPBUF, 1); \
                                            LOG_EVENT(TX_START, *(long*)STM_SELF, __FILE__, __LINE__); \
                                            TxStart(STM_SELF, &STM_JMPBUF, &STM_RO_FLAG); \
                                        } while (0) /* enforce comma */

#define STM_BEGIN_RD()                  STM_BEGIN(1)
#define STM_BEGIN_WR()                  STM_BEGIN(0)
#define STM_END()                       do { \
                                            if (TxCommit(STM_SELF)) \
                                              LOG_EVENT(TX_COMMIT, *(long*)STM_SELF, __FILE__, __LINE__); \
                                            else \
                                              LOG_EVENT(TX_ABORT, *(long*)STM_SELF, __FILE__, __LINE__); \
                                        } while (0) /* enforce comma */

typedef volatile intptr_t               vintp;

static __attribute__((always_inline)) intptr_t TxReadWrapper(Thread* Self, volatile intptr_t* Addr)
{
  LOG_EVENT(TX_READ, *(long*)STM_SELF, __FILE__, __LINE__);
  return TxLoad(Self, Addr);
}

static __attribute__((always_inline)) void TxWriteWrapper(Thread* Self, volatile intptr_t* addr, intptr_t valu)
{
  LOG_EVENT(TX_WRITE, *(long*)STM_SELF, __FILE__, __LINE__);
  TxStore(Self, addr, valu);
}

#define STM_READ(var)                   TxReadWrapper(STM_SELF, (vintp*)(void*)&(var))
#define STM_READ_F(var)                 IP2F(TxReadWrapper(STM_SELF, \
                                                    (vintp*)FP2IPP(&(var))))
#define STM_READ_P(var)                 IP2VP(TxReadWrapper(STM_SELF, \
                                                     (vintp*)(void*)&(var)))

#define STM_WRITE(var, val)             TxWriteWrapper(STM_SELF, \
                                                (vintp*)(void*)&(var), \
                                                (intptr_t)(val))
#define STM_WRITE_F(var, val)           TxWriteWrapper(STM_SELF, \
                                                (vintp*)FP2IPP(&(var)), \
                                                F2IP(val))
#define STM_WRITE_P(var, val)           TxWriteWrapper(STM_SELF, \
                                                (vintp*)(void*)&(var), \
                                                VP2IP(val))

#define STM_LOCAL_WRITE(var, val)       ({var = val; var;})
#define STM_LOCAL_WRITE_F(var, val)     ({var = val; var;})
#define STM_LOCAL_WRITE_P(var, val)     ({var = val; var;})


#endif /* STM_H */


/* =============================================================================
 *
 * End of stm.h
 *
 * =============================================================================
 */
