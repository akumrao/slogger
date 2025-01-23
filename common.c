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
//log_type_t log_type = LOG_TYPE_ADBSHELL;
int console_loglevel = LOG_INFO;

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