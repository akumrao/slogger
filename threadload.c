

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
#include "condwait.h"




/*
 * For thread load
 */


#define TAG "GPU : Load"


/* The key used to associate a log file pointer with each thread. */
pthread_key_t thread_log_key;




void close_thread_log (void* thread_log)
{
    
    long fsize = ftell(thread_log);
   
    if(fsize > 0)
    {
        int npos = fseek(thread_log, 0, SEEK_SET);

        char *string = malloc(fsize + 1+2);
        if(string)
        {
           int sz = fread(string, 1, fsize , thread_log);

           string[sz-1]= '\n';
           string[sz-2]= '\n';

           pushMessage(string, sz );
        }
        //free(string);
    }
    fclose ((FILE*) thread_log);
    thread_log = NULL;
}


void* load_thread(void* arg) {

    
    ThLoader *th = (ThLoader *) arg;
    
    //printf(" \n test %s \n", th->logPath);
    
    
    printf("load_thread started\n");
    
    char thread_log_filename[256];
    // FILE* thread_log;

     
     
   /* Generate the filename for this thread.s log file. */
    
    if(th->clubbed)
        sprintf (thread_log_filename, "%sthread%d.log", th->logPath, (int) pthread_self ());
    else
       sprintf (thread_log_filename, "%s%s", th->logPath, th->logfile); 

    th->thread_log = fopen (thread_log_filename, "w+");
    if (th->thread_log == NULL) {
          fprintf(stderr, "Failed to open file\n");
          return NULL;
    }
  

    /* Store the file pointer in thread-specific data under thread_log_key. */
   // pthread_setspecific (thread_log_key, thread_log);
   //write_to_thread_log ("Thread starting.");                   
  
    th->run(th);
    

    if(th->clubbed)
    {
        log_message(LOG_DEBUG, th->thread_log, TAG, "End of thread tid = %ld ", (long) pthread_self () );
        close_thread_log(th->thread_log);
        
    }
    else
    {
        slog_message(LOG_DEBUG, TAG, "End of thread %s ",  (long) pthread_self () );
        
        fclose (th->thread_log);
        
    }
    
    
    fflush(stdout);
    
    return NULL;
}



void thload_start(ThLoader* th){ 
    
   // pthread_t threads;
    th->keeprunning = 1;
  //  pthread_key_create (&thread_log_key, close_thread_log);  // this line make the thread slow, please do not enable it
    pthread_create(&th->threads, NULL, load_thread, th);
    
} 

void thload_run_cond(ThLoader* th){ 
    
 
     
    int ncount = 0;
    while (atomic_load_explicit(&th->keeprunning, memory_order_relaxed))
    {
        Condwait condwait = (Condwait){  condwait_wait, condwait_signal, condwait_stop }; 
          
   
        
        if(th->clubbed)
          log_message(LOG_DEBUG, th->thread_log, TAG, "GPU001 load %d , tid = %ld ", ncount++,  (long) pthread_self () );
        else
          slog_message(LOG_DEBUG,  TAG, "GPU001 load %d , tid = %ld ",  ncount++,  (long) pthread_self () );

        
        
        condwait.wait(&condwait, 0, 100);
        
        condwait.stop(&condwait);
    }
   
    
} 


void thload_run(ThLoader* th){ 
    
    int ncount = 0;
    while (atomic_load_explicit(&th->keeprunning, memory_order_relaxed))
    {
        if(th->clubbed)
           log_message(LOG_DEBUG, th->thread_log, TAG, "GPU001 load %d , tid = %ld ", ncount++,  (long) pthread_self () );
        else
           slog_message(LOG_DEBUG, TAG, "GPU001 load %d , tid = %ld ",  ncount++,  (long) pthread_self () );
        
        usleep(10000);
    }
   
} 
 

void thload_stop(ThLoader* th){ 
    
     
    printf("thload_stop\n" );
    
    atomic_store_explicit(&th->keeprunning,0 , memory_order_relaxed);

    pthread_join(th->threads, NULL);

   
} 

