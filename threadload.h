/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threadload.h
 * Author: aumrao
 *
 * Created on January 23, 2025, 11:25 AM
 */


#ifndef THREADLOAD_H
#define THREADLOAD_H

#include <pthread.h>
#include <stdatomic.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _threadload { 

  void (*start)( struct _threadload*);
  void (*run)( struct _threadload*); 
  void (*stop)( struct _threadload*); 
  
  char logPath[MAX_PATH_LENGTH];
  atomic_bool keeprunning;  
  
  pthread_t threads; 
  
} ThLoader ; 

    
void thload_start(ThLoader* th);

void thload_run(ThLoader* th);

void thload_stop(ThLoader* th);

#ifdef __cplusplus
}
#endif

#endif /* THREADLOAD_H */

