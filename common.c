/*
 * Copyright (c) 2024-25 Google SLT Team
 *
 * Filename:        common.c
 * Description:     Implements common functions and global variables.
 *                  This file provides implementations for several core
 *                  functionalities used across the subsystems:
 *
 * Author:          Arvind Umrao <aumrao@google.com>
 *                  
 */

#include "common.h"
#include <sys/time.h>

#if defined(__ANDROID__) 
#include <android/log.h>
#endif

extern FILE *logcat_fp;

/**
 * Function:    log_message
 * Description: Log a message to the appropriate output based on the current logging type.
 *
 * param:       log_lvl- The log level of the message (e.g., LOG_INFO, LOG_ERR, LOG_DEBUG).
 *              tag- string representing the tag or context identifier for the log message.
 *              fp- A file pointer for additional file logging (can be NULL).
 *              fmt- A format string for the log message (similar to printf).
 *              ... Additional arguments for the format string.
 * return:      None
 */
void log_message(int log_lvl, FILE *fp, const char *tag, const char *fmt, ...)
{
    if (log_lvl <= console_loglevel) {
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
                switch (log_lvl) {
                    #if defined(__ANDROID__) 
                    case LOG_INFO: __android_log_vprint(ANDROID_LOG_INFO, tag, fmt, args); break;
                    case LOG_ERR: __android_log_vprint(ANDROID_LOG_ERROR, tag, fmt, args); break;
                    case LOG_DEBUG: __android_log_vprint(ANDROID_LOG_WARN, tag, fmt, args); break;
                    default: __android_log_print(ANDROID_LOG_WARN, tag, "UNKNOWN: %s", fmt); break;
                    #endif    
                }
            case LOG_TYPE_ADBSHELL:
                switch (log_lvl) {
                    case LOG_INFO: printf(COLOR_BLUE "%s.%03d  INFO %s: ", timestamp, msec, tag); VAR_LOG; break;
                    case LOG_ERR: printf(COLOR_RED "%s.%03d ERROR %s: ", timestamp, msec, tag); VAR_LOG; break;
                    case LOG_DEBUG: printf(COLOR_YELLOW "%s.%03d DEBUG %s: ", timestamp, msec, tag); VAR_LOG; break;
                    default: printf(COLOR_GREEN "%s.%03d  WARN %s: ", timestamp, msec, tag); VAR_LOG; break;
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
                case LOG_INFO: fprintf(fp, "%s.%03d  INFO %s:  ", timestamp, msec, tag); break;
                case LOG_ERR: fprintf(fp, "%s.%03d ERROR %s:  ", timestamp, msec, tag); break;
                case LOG_DEBUG: fprintf(fp, "%s.%03d DEBUG %s:  ", timestamp, msec, tag); break;
                default: fprintf(fp, "%s.%03d  WARN %s:  ", timestamp, msec, tag); break;
            }
            va_start(args, fmt); /* Restart the variable argument list */
            vfprintf(fp, fmt, args); fprintf(fp, "\n"); fflush(fp);
            va_end(args);
        }
    }
}

/**
 * Function:    set_log_type
 * Description: Set the logging type based on the provided configuration value.
 * param:       config_value A string representing the desired log mode
 *              ("console - for adb shell", "logcat", or "serial").
 * return:      Return SUCCESS | FAIL
 */
int set_log_type(const char *config_value)
{
    if (strcmp(config_value, "adbshell") == 0) {
        log_type = LOG_TYPE_ADBSHELL;
    } else if (strcmp(config_value, "logcat") == 0) {
        log_type = LOG_TYPE_LOGCAT;
    } else if (strcmp(config_value, "serial") == 0) {
        log_type = LOG_TYPE_SERIAL;
    } else {
        //log_message(LOG_WARN, log_fp,  "Invalid log mode: %s. Defaulting to console.",FL, config_value);
        log_type = LOG_TYPE_ADBSHELL;
    }
    return SUCCESS;
}



#define LOG_BUFFER_SIZE 1024

/**
 * Function: void log_message(int log_lvl, FILE *fp,
 *                             const char *tag, const char *fmt, ...)
 * Description: Logs a message to the console and/or a
 *              file, depending on the log level, log
 *              type, and file pointer. It formats the
 *              message with a timestamp and tag.
 * Parameters:
 *   - log_lvl: int - The log level of the message
 *     (e.g., LOG_INFO, LOG_ERR, LOG_DEBUG).
 *   - fp: FILE * - A file pointer to log to a file,
 *     or NULL to log only to the console.
 *   - tag: const char * - A tag to identify the
 *     source of the log message.
 *   - fmt: const char * - The format string for the
 *     log message.
 *   - ...: Variable arguments to be formatted
 *     according to the format string.
 * Returns:
 *   void - This function does not return a value.
 */
void slog_message(int log_lvl, const char *tag, const char *fmt, ...)
{
    
    if (log_lvl <= console_loglevel) {
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
                switch (log_lvl) {
                    #if defined(__ANDROID__) 
                    case LOG_INFO: __android_log_vprint(ANDROID_LOG_INFO, tag, fmt, args); break;
                    case LOG_ERR: __android_log_vprint(ANDROID_LOG_ERROR, tag, fmt, args); break;
                    case LOG_DEBUG: __android_log_vprint(ANDROID_LOG_WARN, tag, fmt, args); break;
                    default: __android_log_print(ANDROID_LOG_WARN, tag, "UNKNOWN: %s", fmt); break;
                    #endif    
                }
            case LOG_TYPE_ADBSHELL:
                switch (log_lvl) {
                    case LOG_INFO: printf(COLOR_BLUE "%s.%03d  INFO %s: ", timestamp, msec, tag); VAR_LOG; break;
                    case LOG_ERR: printf(COLOR_RED "%s.%03d ERROR %s: ", timestamp, msec, tag); VAR_LOG; break;
                    case LOG_DEBUG: printf(COLOR_YELLOW "%s.%03d DEBUG %s: ", timestamp, msec, tag); VAR_LOG; break;
                    default: printf(COLOR_GREEN "%s.%03d  WARN %s: ", timestamp, msec, tag); VAR_LOG; break;
                }
                break;
            case LOG_TYPE_SERIAL:
                break;
            default:
                break;
        }
        va_end(args);

         char *store = (char *)malloc(2048);
         if(!store)
              return;
         
        int ncount = 0;
        
        /* Log to file if the file pointer is not NULL */
       // if (fp != NULL) 
        {
            switch (log_lvl) {
                case LOG_INFO: ncount += sprintf(&store[ncount], "%s.%03d  INFO %s:  ", timestamp, msec, tag); break;
                case LOG_ERR: ncount += sprintf(&store[ncount], "%s.%03d ERROR %s:  ", timestamp, msec, tag); break;
                case LOG_DEBUG: ncount += sprintf(&store[ncount], "%s.%03d DEBUG %s:  ", timestamp, msec, tag); break;
                default: ncount += sprintf(&store[ncount], "%s.%03d  WARN %s:  ", timestamp, msec, tag); break;
            }
            va_start(args, fmt); /* Restart the variable argument list */
            ncount += vsprintf(&store[ncount], fmt, args);  ncount += sprintf(&store[ncount], "\n"); //fflush(fp);
            va_end(args);
        }
        pushMessage(store , strlen(store) );
    }
}
