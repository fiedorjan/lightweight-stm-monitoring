/**
 * @brief Contains definitions shared among various parts of the library.
 *
 * A file containing definitions shared among various parts of the library.
 *
 * @file      defs.h
 * @author    Jan Fiedor (fiedorjan@centrum.cz)
 * @date      Created 2014-06-20
 * @date      Last Update 2014-06-23
 * @version   0.1.3
 */

#ifndef __DEFS_H__
  #define __DEFS_H__

typedef long thread_id_t;
typedef unsigned int tx_type_t;
typedef unsigned long tx_op_counter_t;
typedef unsigned long long timestamp_t;

#ifdef __cplusplus
  #define API_FUNCTION extern "C"
#else
  #define API_FUNCTION
#endif

#endif /* __DEFS_H__ */

/** End of file defs.h **/
