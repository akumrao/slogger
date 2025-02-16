/*
 * Authored Arvind &  Rajnee  
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   condwait.h
 * Author: aumrao
 *
 * Created on January 23, 2025, 11:25 AM
 */

#include <pthread.h>

#ifndef CONDWAIT_H
#define CONDWAIT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _condwait { 
  
  void (*init)( struct _condwait* ); 
     
  int (*wait)( struct _condwait*, int, int ); 
  
  void (*signal)( struct _condwait*); 
  
  void (*stop)( struct _condwait*); 
    
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  struct timespec timeout;
     
} Condwait ; 

void condwait_int(Condwait* th);

int condwait_wait(Condwait* th, int timeInSec, int timeInMs);

void condwait_signal(Condwait* th);

void condwait_stop(Condwait* th);

#ifdef __cplusplus
}
#endif

#endif /* CONDWAIT_H */

