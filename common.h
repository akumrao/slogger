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
#include <signal.h>

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_ERR     0   /* error conditions */
#define LOG_WARN    1   /* warning conditions */
#define LOG_INFO    2   /* informational */
#define LOG_DEBUG   3   /* debug-level messages */

 

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

/* Config file default arguments required across the modules */
struct default_config {
  char test_id[MAX_TYPE_LENGTH];
  char board_name[MAX_TYPE_LENGTH];
  char kernel_version[MAX_TYPE_LENGTH];
  char build_version[MAX_TYPE_LENGTH];
  bool power;
  bool voltage;
  bool temperature;
  bool frequency;
  int log_interval;
  char log_mode[MAX_TYPE_LENGTH];
  char output_path[MAX_PATH_LENGTH];
};

/* Structure to hold power rail data */
typedef struct {
  long int timestamp;
  long int energy_value;
  char rail_name[MAX_TYPE_LENGTH];
} power_data_point_t;

typedef struct {
  volatile sig_atomic_t *terminate;
  const char *tag;
} logcat_args_t;

/* Define an enum for your custom thread error codes */
enum thread_error_code {
  THREAD_SUCCESS = 0,
  THREAD_ERROR_FOPEN = 1,
  THREAD_ERROR_POPEN = 2,
  THREAD_ERROR_PARAMETER = 3,
  THREAD_ERROR_ALLOC_MEM = 4,
  THREAD_ERROR_MEMCPY = 5,
};    
    
typedef struct {
  enum thread_error_code code;
  char *message;
} thread_error_info_t;

/* Global config for logging type */
typedef enum { LOG_TYPE_ADBSHELL, LOG_TYPE_LOGCAT, LOG_TYPE_SERIAL } log_type_t;

/* Global variable to store the console log level. */
extern int console_loglevel;
/* This flag is set based on the config */
extern log_type_t log_type;
/* File pointer for test log */
extern FILE *log_fp;
extern FILE *logcat_fp;

/* Gets the current timestamp and stores it in the provided string */
void get_timestamp(char *timestamp_str, size_t str_size);

void log_message(int log_lvl, FILE* fp,  const char *tag, const char *fmt, ...) ;

#ifdef __cplusplus
}
#endif

#endif /* SLOGGER_H */

