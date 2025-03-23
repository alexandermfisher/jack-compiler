//
// Created by alexanderfisher on 23/03/25.
//
#include "logger.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

// Optional color codes
static const char *level_colors[] = {
    "\x1b[36m", // DEBUG - Cyan
    "\x1b[32m", // INFO  - Green
    "\x1b[33m", // WARN  - Yellow
    "\x1b[31m" // ERROR - Red
};

static const char *level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};

// Global logger instance
static Logger *global_logger = NULL;

Logger *logger_create(const char *log_filepath, const LogLevel level, const bool use_colors) {
    Logger *logger = calloc(1, sizeof(Logger));
    if (!logger) return NULL;

    logger->level = level;
    logger->use_colors = use_colors;

    if (log_filepath) {
        logger->stream = fopen(log_filepath, "w");
        if (!logger->stream) {
            free(logger);
            return NULL;
        }
    } else {
        logger->stream = open_memstream(&logger->mem_buffer, &logger->mem_size);
        if (!logger->stream) {
            free(logger);
            return NULL;
        }
    }

    return logger;
}

void logger_log(Logger *logger, LogLevel level, const char *file, int line, const char *fmt, ...) {
    if (!logger || level < logger->level) return;

    // Timestamp
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    // Color if enabled
    if (logger->use_colors) fprintf(logger->stream, "%s", level_colors[level]);

    // Log format start
    fprintf(logger->stream, "[%s] %s", time_buf, level_names[level]);

    // Conditionally print user file and line if provided
    if (file && line > 0) {
        fprintf(logger->stream, " [%s:%d]", file, line);
    }

    fprintf(logger->stream, ": ");

    // Formatted message
    va_list args;
    va_start(args, fmt);
    vfprintf(logger->stream, fmt, args);
    va_end(args);

    fprintf(logger->stream, "\n");
    if (logger->use_colors) fprintf(logger->stream, "\x1b[0m"); // Reset color
}

void logger_dump(Logger *logger, FILE *target) {
    if (!logger || !logger->mem_buffer || !target) return;
    fflush(logger->stream); // Ensure all data is written to buffer
    fwrite(logger->mem_buffer, 1, logger->mem_size, target);
}

void logger_free(Logger *logger) {
    if (!logger) return;
    fflush(logger->stream);
    fclose(logger->stream);
    free(logger->mem_buffer);
    free(logger);
}

// Global logger setters/getters
void logger_set_global(Logger *logger) {
    global_logger = logger;
}

Logger *logger_get_global(void) {
    return global_logger;
}
