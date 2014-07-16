/**
 * @brief Contains configuration influencing the compilation of the library.
 *
 * A file containing configuration influencing the compilation of the library.
 *
 * @file      config.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-19
 * @date      Last Update 2014-07-16
 * @version   0.2.4
 */

#ifndef __CONFIG_H__
  #define __CONFIG_H__

#define LWM_FAST_PER_TX_TYPE_ABORTS 1
#define LWM_EVT_LOG_PER_TX_TYPE_ABORTS 2
#define LWM_EVT_LOG_PER_THREAD_ABORTS 3

#define LWM_TYPE LWM_EVT_LOG_PER_THREAD_ABORTS

// Currently only valid if LWM_TYPE is LWM_EVT_LOG_PER_THREAD_ABORTS
#define LWM_COLLECT_TIMESTAMPS 1
// Currently only valid if LWM_TYPE is LWM_EVT_LOG_PER_THREAD_ABORTS
#define LWM_TRACK_ABORTS 1
// Currently only valid if LWM_TYPE is LWM_EVT_LOG_PER_THREAD_ABORTS
#define LWM_TRACK_READS 1
// Currently only valid if LWM_TYPE is LWM_EVT_LOG_PER_THREAD_ABORTS
#define LWM_TRACK_WRITES 1

// Currently only valid if LWM_TYPE is LWM_EVT_LOG_PER_THREAD_ABORTS and LWM_COLLECT_TIMESTAMPS is 1
#define LWM_COMPUTE_AVG_TX_LENGTH 1
// Currently only valid if LWM_TYPE is LWM_EVT_LOG_PER_THREAD_ABORTS and LWM_COLLECT_TIMESTAMPS is 1
#define LWM_COMPUTE_TX_UTILIZATION 1

#define LWM_MAX_THREADS 8
#define LWM_MAX_TX_TYPES 32

#endif /* __CONFIG_H__ */

/** End of file config.h **/
