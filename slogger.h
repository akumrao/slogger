/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   slogger.h
 * Author: aumrao
 *
 * Created on January 23, 2025, 11:25 AM
 */

#include <pthread.h>

#ifndef SLOGGER_H
#define SLOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_ERR     0   /* error conditions */
#define LOG_WARN    1   /* warning conditions */
#define LOG_INFO    2   /* informational */
#define LOG_DEBUG   3   /* debug-level messages */

#define BUFFER_SIZE 1024    
    
typedef struct _slogger { 
   
  pthread_t *threads; 
  void (*start)( struct _slogger*); 
  void (*stop)( struct _slogger*); 
     
} Slogger ; 

    
void slog_start(Slogger* th);

void slog_stop(Slogger* th);

#ifdef __cplusplus
}
#endif

#endif /* SLOGGER_H */

