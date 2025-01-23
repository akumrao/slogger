/*
 * Copyright (c) 2024-25 Google SLT Team
 *
 * Filename:        common.h
 * Description:     Common functionality defines here to use across the modules.
 *                  It consist of common functionality which used across the
 *                  all the subsystem test cases.
 *
 * Author:          Amit Kumar <amittkumarrr@google.com>
 */

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_ERR     0   /* error conditions */
#define LOG_WARN    1   /* warning conditions */
#define LOG_INFO    2   /* informational */
#define LOG_DEBUG   3   /* debug-level messages */

#define BUFFER_SIZE 1024    

#define COLOR_RESET "\033[0m\n"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"

#define EFMT "[%s:%d] "
//#define FL __FILE_NAME__, __LINE__
#define SUCCESS 0
#define FAILURE 1
#define DEFAULT_LOG_INTERVAL 200 /* Default log interval */
#define MAX_LOG_INTERVAL 1000    /* Max log value 1 sec */
#define TRUE 1
#define FALSE 0
#define MAX_PATH_LENGTH 128
#define MAX_TYPE_LENGTH 32
#define THREAD_FAILURE (void *)-1


/* Gets the current timestamp and stores it in the provided string */
void get_timestamp(char *timestamp_str, size_t str_size);

#ifdef __cplusplus
}
#endif

#endif /* SLOGGER_H */

