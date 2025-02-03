/*
 * Copyright (c) 2024-25 Google SLT Team
 *
 * Filename:        common.c
 * Description:     Implements common functions and global variables.
 *                  This file provides implementations for several core
 *                  functionalities used across the subsystems:
 *
 * Author:          Amit Kumar <amittkumarrr@google.com>
 *                  Ranjeet Kumar <kumarnjeet@google.com>
 */

#include "common.h"

#define BUFFER_SIZE 64
#define VAR_LOG        \
  vprintf(fmt, args);  \
  printf(COLOR_RESET); \
  fflush(stdout)
#define DEBUGFS_MOUNT_POINT "/sys/kernel/debug"
#define BUILD_VERSION_CMD "getprop ro.product.build.version.incremental"

/* Global variable used across the modules */
FILE *log_fp = NULL;
FILE *logcat_fp = NULL; /* Logcat file pointer */
log_type_t log_type = LOG_TYPE_ADBSHELL;
int console_loglevel = LOG_DEBUG;

/**
 * Function: void get_timestamp(char *timestamp_str, size_t str_size)
 * Description: Retrieves the current timestamp with
 *              millisecond precision and formats it into
 *              a string. The format is
 *              "Mon-DD_HH:MM:SS.mmm".
 * Parameters:
 *   - timestamp_str: char * - A buffer to store the formatted timestamp string.
 *   - str_size: size_t - The size of the timestamp buffer.
 * Returns:
 *   void - This function does not return a value.
 */
void get_timestamp(char *timestamp_str, size_t str_size) {
  int msec = 0;
  char timestr[BUFFER_SIZE] = {0};

  time_t now = time(NULL);
  struct tm *local_time = localtime(&now);
  struct timeval tv;
  gettimeofday(&tv, NULL);

  msec = tv.tv_usec / 1000;
  strftime(timestr, sizeof(timestr), "%b-%d_%H:%M:%S", local_time);
  snprintf(timestamp_str, str_size, "%s.%03d", timestr, msec);
}



/**
 * Function:    void log_message(int log_lvl, FILE *fp, const char *tag,
 *              const char *fmt, ...)
 * Description: Log a message to the appropriate output based
 *              on the current logging type.
 *
 * Parameters:  int log_lvl- The log level of the message
 *              (e.g., LOG_INFO, LOG_ERR, LOG_DEBUG).
 *              const char *tag- string representing the tag or
 *              context identifier
 *              for the log message.
 *              FILE *fp- A file pointer for additional file
 *              logging (can be NULL).
 *              const char *fmt- A format string for the log message
 *              (similar to printf).
 *              ... Additional arguments for the format string.
 * Returns:      None
 */

extern pthread_key_t thread_log_key;

void th_log_message(int log_lvl, const char *tag, const char *fmt, ...) {
  if (log_lvl <= console_loglevel) {
      
    FILE* fp = (FILE*) pthread_getspecific (thread_log_key);
     
    va_list args;
    va_start(args, fmt);

    /* Time and milliseconds for timestamp */
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int msec = tv.tv_usec / 1000;
    char timestamp[80];
    strftime(timestamp, sizeof(timestamp), "%b-%d %H:%M:%S", local_time);

    switch (log_type) {
      case LOG_TYPE_LOGCAT:
      case LOG_TYPE_ADBSHELL:
        switch (log_lvl) {
          case LOG_INFO:
            printf(COLOR_BLUE "%s.%03d  INFO %s: ", timestamp, msec, tag);
            VAR_LOG;
            break;
          case LOG_ERR:
            printf(COLOR_RED "%s.%03d ERROR %s: ", timestamp, msec, tag);
            VAR_LOG;
            break;
          case LOG_DEBUG:
            printf(COLOR_YELLOW "%s.%03d DEBUG %s: ", timestamp, msec, tag);
            VAR_LOG;
            break;
          default:
            printf(COLOR_GREEN "%s.%03d  WARN %s: ", timestamp, msec, tag);
            VAR_LOG;
            break;
        }
        break;
      case LOG_TYPE_SERIAL:
        break;
      default:
        break;
    }
    va_end(args);

    /* Log to file if the file pointer is not NULL */
    if (fp != NULL) {
      switch (log_lvl) {
        case LOG_INFO:
          fprintf(fp, "%s.%03d  INFO %s:  ", timestamp, msec, tag);
          break;
        case LOG_ERR:
          fprintf(fp, "%s.%03d ERROR %s:  ", timestamp, msec, tag);
          break;
        case LOG_DEBUG:
          fprintf(fp, "%s.%03d DEBUG %s:  ", timestamp, msec, tag);
          break;
        default:
          fprintf(fp, "%s.%03d  WARN %s:  ", timestamp, msec, tag);
          break;
      }
      va_start(args, fmt); /* Restart the variable argument list */
      vfprintf(fp, fmt, args);
      fprintf(fp, "\n");
      fflush(fp);
      va_end(args);
    }
  }
}