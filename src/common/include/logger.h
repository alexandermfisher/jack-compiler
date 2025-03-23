#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

// Log levels
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

typedef struct {
    FILE *stream;         // File or memory stream
    char *mem_buffer;     // Buffer if using memory stream
    size_t mem_size;      // Size of the memory buffer
    LogLevel level;       // Current log level threshold
    bool use_colors;      // Enable ANSI colors for terminal
} Logger;

// Create a new logger
Logger *logger_create(const char *log_filepath, LogLevel level, bool use_colors);

// Log a message (internal usage)
void logger_log(Logger *logger, LogLevel level, const char *file, int line, const char *fmt, ...);

// Dump memory log buffer to a target stream (if using memory logging)
void logger_dump(Logger *logger, FILE *target);

// Free logger resources
void logger_free(Logger *logger);

// Set or get the global logger instance
void logger_set_global(Logger *logger);
Logger *logger_get_global(void);

// General user log - no source file or line shown
#define GLOG(level, ...) \
    do { \
        if (logger_get_global()) \
            logger_log(logger_get_global(), level, NULL, 0, __VA_ARGS__); \
    } while (0)

// User error log pointing to their source file and line number
#define GUSERLOG(level, user_file, user_line, ...) \
    do { \
        if (logger_get_global()) \
            logger_log(logger_get_global(), level, user_file, user_line, __VA_ARGS__); \
    } while (0)

#endif // LOGGER_H
