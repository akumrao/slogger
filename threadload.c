

#include "common.h"
#include "threadload.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <string.h>
#include <signal.h>





/*
 * For thread load
 */


#define TAG "UFS : Load"


/* The key used to associate a log file pointer with each thread. */
pthread_key_t thread_log_key;


void write_to_thread_log (const char* message)
{
  FILE* thread_log = (FILE*) pthread_getspecific (thread_log_key);
  fprintf (thread_log, "%s\n", message);
}

void close_thread_log (void* thread_log)
{
    
  fclose ((FILE*) thread_log);
  
}



void* load_thread(void* arg) {

    
    ThLoader *th = (ThLoader *) arg;
    
    printf(" \n test %s \n", th->logPath);
    
    
    printf("load_thread started\n");
    
    char thread_log_filename[256];
     FILE* thread_log;

   /* Generate the filename for this thread.s log file. */
    sprintf (thread_log_filename, "%sthread%d.log", th->logPath, (int) pthread_self ());


    thread_log = fopen (thread_log_filename, "w");
    if (thread_log == NULL) {
          fprintf(stderr, "Failed to open file\n");
          return NULL;
    }
  

    /* Store the file pointer in thread-specific data under thread_log_key. */
    pthread_setspecific (thread_log_key, thread_log);
   //write_to_thread_log ("Thread starting.");                   
  
    th->run(th);

     
    printf("End of load_thread \n");
    
    fflush(stdout);
    
    return NULL;
}



void thload_start(ThLoader* th){ 
    
   // pthread_t threads;
    th->keeprunning = 1;
    pthread_key_create (&thread_log_key, close_thread_log);

    pthread_create(&th->threads, NULL, load_thread, th);
  
    
} 

void thload_run(ThLoader* th){ 
    
    int ncount = 0;
    while (atomic_load_explicit(&th->keeprunning, memory_order_relaxed))
    {
        th_log_message(LOG_DEBUG, TAG, "UFS001 load %d , tid = %ld ", ncount++,  (long) pthread_self () );
        usleep(1000);
    }
   
} 

 

void thload_stop(ThLoader* th){ 
    
     
    printf("thload_stop\n" );
    
    atomic_store_explicit(&th->keeprunning,0 , memory_order_relaxed);

    pthread_join(th->threads, NULL);

   
} 

