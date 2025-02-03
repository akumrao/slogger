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

#define LOG_BUFFER_SIZE 1024    
#define MAX_PATH_LEN 255
    
typedef struct _slogger { 
   
  void (*start)( struct _slogger*); 
  void (*stop)( struct _slogger*); 
  
  char logPath[MAX_PATH_LEN];
  
  pthread_t *threads; 
  
} Slogger ; 

    
void slog_start(Slogger* th);

void slog_stop(Slogger* th);

#ifdef __cplusplus
}
#endif

#endif /* SLOGGER_H */

