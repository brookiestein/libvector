#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void logger(enum LEVEL level, bool log, const char *func, int line, const char *format, ...)
{
    // This condition is to not perform the same check every time in vector's functions.
    if (!log)
        return;

    const char *tmpfile = "/tmp/vector.log";
    const size_t size = 255;
    FILE *fp = fopen(tmpfile, "w+");
    char *message = (char *) malloc(size * sizeof(char));
    va_list args;

    va_start(args, format);
    if (vfprintf(fp, format, args) < 0) {
        fprintf(stderr, "There was an error parsing arguments.\n");
        return;
    }
    va_end(args);

    fseek(fp, SEEK_SET, SEEK_SET);
    message = fgets(message, size, fp);

    FILE *stream = level == ERROR ? stderr : stdout;

    fprintf(stream, "%s: %s():%i: %s\n", get_log_level(level), func, line, message);
    free(message);
    fclose(fp);
    remove(tmpfile);
}

const char *get_log_level(enum LEVEL level)
{
    switch (level) {
    case INFO:
        return "INFO";
    case WARN:
        return "WARN";
    case ERROR:
        return "ERROR";
    }
}
