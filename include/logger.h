#ifndef LOG_H
#define LOG_H

#include <stdbool.h>
#include <stdio.h>

enum LEVEL {
    INFO,
    WARN,
    ERROR
};

const char *get_log_level(enum LEVEL level);
void logger(
        enum LEVEL level,
        bool do_log,
        const char *func,
        int line,
        const char *format,
        ...);

#endif // LOG_H
