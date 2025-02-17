/*
 * Copyright (c) 2024-25 Google SLT Team
 *
 * FileName:      threadload.h
 * Description:   This header file defines the structure and function 
 *                declarations for managing threads in GPU test cases.
 * Author:        Arvind Umrao <aumrao@google.com> 
 *                Rajanee Kumbhar <rajaneek@google.com>
 */

#ifndef THREADLOAD_H
#define THREADLOAD_H

#include <pthread.h>

#ifdef __cplusplus
#include <atomic>
using namespace std;
#else
#include <stdatomic.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif
#define MAX_PATH_LENGTH 128

typedef struct _threadload { 

  void (*start)( struct _threadload*);
  void (*run)( struct _threadload*); 
  void (*stop)( struct _threadload*); 
  
  bool clubbed; // thread clubbed
  char logPath[MAX_PATH_LENGTH]; //for eg /tmp/
  char logfile[MAX_PATH_LENGTH]; //UFS001.txt  


    #ifdef __cplusplus
    //    atomic<bool> keeprunning{1};
    volatile int keeprunning;
    #else
    atomic_bool keeprunning ;
    #endif

    bool localFile;

  pthread_t threads; 
  
  FILE *thread_log;
  
} ThLoader ; 

    
void thload_start(ThLoader* th);

void thload_run(ThLoader* th);

void thload_stop(ThLoader* th);

#ifdef __cplusplus
}
#endif

#endif /* THREADLOAD_H */

