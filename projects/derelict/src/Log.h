//
// Created by johan on 2026-10-05.
//

#ifndef GENESIS_DOES_LOGGER_H
#define GENESIS_DOES_LOGGER_H

#define LOG_LEVEL_OFF   0
#define LOG_LEVEL_INF  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERR   3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_TRACE 5

#define LOG_LEVEL (LOG_LEVEL_TRACE)

#if LOG_LEVEL >= LOG_LEVEL_INF
#define LOG_info(M, ...) kprintf("[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
# else
#define LOG_info(M, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_debug(M, ...) kprintf("[DEBUG] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
# else
#define LOG_debug(M, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_TRACE
#define LOG_trace(M, ...) kprintf("[TRACE] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
# else
#define LOG_trace(M, ...)
#endif

#endif //GENESIS_DOES_LOGGER_H
