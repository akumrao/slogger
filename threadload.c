/*
 * Copyright (c) 2024-25 Google SLT Team
 *
 * FileName:      threadload.c
 * Description:   File have common code wrapper implementation to gpu.
 * Author:        Arvind Umrao <aumrao@google.com> 
 *                
 */

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

/**
 * Function: void close_thread_log(FILE* thread_log)
 * Description: Closes the log file associated with the given
 *              thread and processes the log data before closure.
 *              The function reads the contents of the log file,
 *              adds newline characters at the end, and pushes 
 *              the processed message using the `pushMessage` function.
 * Parameters:
 *   - thread_log: FILE* - A pointer to the log file to be closed and processed.
 * Returns:
 *   void - No return value. The function modifies the log file contents
 *          and performs cleanup.
 */
void close_thread_log (FILE* thread_log) {
    long fsize = ftell(thread_log);
   
    if(fsize > 0)
    {
        fseek(thread_log, 0, SEEK_SET);

        char *string =  (char *)malloc(fsize + 1+2);
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

/**
 * Function: void* load_thread(void* arg)
 * Description: Initializes a thread's log file based on `logPath` and 
 *              either the thread's ID or a specified filename (`logfile`), 
 *              then opens the log file and assigns the file pointer to 
 *              the `ThLoader` structure.
 * Parameters:
 *   - arg: void* - Pointer to a `ThLoader` structure containing the 
 *     log file information.
 * Returns:
 *   void* - Returns NULL if the file can't be opened, otherwise 
 *           continues execution.
 */
void* load_thread(void * arg) {
    ThLoader *th = (ThLoader *) arg;
    printf("load_thread started\n");
    
    char thread_log_filename[256]={'\0'};
     
   /* Generate the filename for this thread.s log file. */

    if(strlen(th->logfile))
        th->localFile = true;
    else
        th->localFile = false;


    if(th->localFile )
        sprintf (thread_log_filename, "%s%s", th->logPath, th->logfile);
    else if(th->clubbed)
        sprintf (thread_log_filename, "%sthread%d.log", th->logPath, (int) pthread_self ());

    if(strlen(thread_log_filename)) {
        th->thread_log = fopen(thread_log_filename, "w+");
        if (th->thread_log == NULL) {
            fprintf(stderr, "Failed to open file\n");
            return NULL;
        }
    }

    /* Store the file pointer in thread-specific data under thread_log_key. */
    // pthread_setspecific (thread_log_key, thread_log);
    // write_to_thread_log ("Thread starting.");                   
  
    th->run(th);
    if(th->thread_log)
    {
        slog_message(LOG_INFO, TAG, "End of thread %s ",  (long) pthread_self () );
        fclose (th->thread_log);
    }
    else if(th->clubbed)
    {
        log_message(LOG_INFO, th->thread_log, TAG, "End of thread tid = %ld ", (long) pthread_self () );
        close_thread_log(th->thread_log);
    }
    else
    {
        slog_message(LOG_INFO, TAG, "End of thread %s ",  (long) pthread_self () );
    }
   // fflush(stdout);
    return NULL;
}

/**
 * Function: void thload_start(ThLoader* th)
 * Description: Starts a new thread that executes the `load_thread` function,
 *              setting the `keeprunning` flag to 1. The function allows 
 *              the thread to run concurrently without waiting for it to finish.
 * Parameters:
 *   - th: ThLoader* - Pointer to a `ThLoader` structure with necessary information.
 * Returns:
 *   void - No return value. The function initiates a new thread and sets the 
 *          `keeprunning` flag.
 */
void thload_start(ThLoader* th){ 
    
   // pthread_t threads;
    th->keeprunning = 1;
  //  pthread_key_create (&thread_log_key, close_thread_log);  // this line make the thread slow, please do not enable it
    pthread_create(&th->threads, NULL, load_thread, th); 
} 

/**
 * Function: void thload_run_cond(ThLoader* th)
 * Description: Runs a loop that logs messages and waits for a condition 
 *              to be met, based on the `keeprunning` flag. The loop 
 *              continues until the flag is set to false.
 * Parameters:
 *   - th: ThLoader* - Pointer to a `ThLoader` structure controlling 
 *     the loop execution.
 * Returns:
 *   void - No return value. The function keeps running until `keeprunning`
 *          is false.
 */

void thload_run1(ThLoader* th){ 
    
    Condwait condwait = (Condwait){  condwait_int, condwait_wait, condwait_signal, condwait_stop }; 
    
    condwait.init(&condwait);
    
    int ncount = 0;
    while (atomic_load_explicit(&th->keeprunning, memory_order_relaxed))
    {
       
        if(th->clubbed || th->localFile)
          log_message(LOG_INFO, th->thread_log, TAG, "GPU001 load %d , tid = %ld ", ncount++,  (long) pthread_self () );
        else
          slog_message(LOG_INFO,  TAG, "GPU001 load %d , tid = %ld ",  ncount++,  (long) pthread_self () );

        
        
        condwait.wait(&condwait, 0, 100);
       // usleep(10000);
        
        
    }
   
    condwait.stop(&condwait);
} 

/**
 * Function: void thload_run(ThLoader* th)
 * Description: Runs a loop that logs messages with the thread ID and a count,
 *              then pauses for a short duration. The loop continues as long 
 *              as the `keeprunning` flag is true.
 * Parameters:
 *   - th: ThLoader* - Pointer to a `ThLoader` structure controlling the loop 
 *     execution.
 * Returns:
 *   void - No return value. The function runs until `keeprunning` is false.
 */
void thload_run_notused(ThLoader* th){ 
    int ncount = 0;
    while (atomic_load_explicit(&th->keeprunning, memory_order_relaxed))
    {
        if(th->clubbed || th->localFile)
           log_message(LOG_INFO, th->thread_log, TAG, "GPU001 load %d , tid = %ld ", ncount++,  (long) pthread_self () );
        else
           slog_message(LOG_INFO, TAG, "GPU001 load %d , tid = %ld ",  ncount++,  (long) pthread_self () );
        
        usleep(10000);
    }
} 
 
/**
 * Function: void thload_stop(ThLoader* th)
 * Description: Stops the running thread by setting the `keeprunning` flag 
 *              to false and waits for the thread to finish using `pthread_join`.
 * Parameters:
 *   - th: ThLoader* - Pointer to a `ThLoader` structure that controls 
 *     the thread's execution.
 * Returns:
 *   void - No return value. The function halts the thread and waits for it to terminate.
 */
void thload_stop(ThLoader* th){ 

    printf("thload_stop\n" );

    atomic_store_explicit(&th->keeprunning,0 , memory_order_relaxed);

    pthread_join(th->threads, NULL);
} 

/**
 * Function: void thload_run_cond(ThLoader* th)
 * Description: Runs a loop that logs messages and waits for a condition 
 *              to be met, based on the `keeprunning` flag. The loop 
 *              continues until the flag is set to false.
 * Parameters:
 *   - th: ThLoader* - Pointer to a `ThLoader` structure controlling 
 *     the loop execution.
 * Returns:
 *   void - No return value. The function keeps running until `keeprunning`
 *          is false.
 */

void thload_run(ThLoader* th)
{ 
    
    Condwait condwait = (Condwait){  condwait_int, condwait_wait, condwait_signal, condwait_stop }; 
    
    condwait.init(&condwait);
    
    char string[40];
 
    
    FILE *filems = fopen("/sys/class/thermal/cooling_device2/stats/time_in_state_ms", "r");
    setvbuf(filems, NULL, _IONBF, 0);
     
    
    int ncount = 0;
    while (atomic_load_explicit(&th->keeprunning, memory_order_relaxed))
    {
        
        //sprintf(string, "timestamp = %li.%09li sec", condwait.timeout.tv_sec, condwait.timeout.tv_nsec);

        int sz = fread(string, 1, 40 , filems);
        
        slog_message(LOG_INFO,  TAG, "%d %s ",ncount++ ,  string );
       
       // pushMessage(string , sz );
        
        condwait.wait(&condwait, 0, 1);
        
        rewind(filems);
        
    }
   
    fclose(filems);
    
    condwait.stop(&condwait);
} 
