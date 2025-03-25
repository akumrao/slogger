/*
 * 
 *
 * Filename:        Gpu_001.c
 * TestCase:        GPU_001
 * TestCase Name:   GPU Stress Test
 * Description:     This file implement a function to stress the GPU and
 *                  capture the parameter like- temperature, frequency, power
 *                  voltage and measure the GPU performance.
 *                  Capture all logs in output log file for further analysis.
 *
 * Author:          Arvind Umrao <aumrao@google.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <signal.h> //  our new library 


#include "slogger.h"
#include "common.h"
#include "condwait.h"
#include "threadload.h"
#include "spwan.h"

#include "playlinklist.h"


#define TAG "GPU : Benchmark"

int console_loglevel = LOG_INFO;          /* Default logging level for console output */
log_type_t log_type;                      /* Variable holds type of log have to be used */
FILE *logcat_fp;                              /* Logcat file pointer */

 Condwait condwait = (Condwait){ condwait_int, condwait_wait, condwait_signal, condwait_stop }; 
 

void intHandler(int sig)
{ // can be called asynchronously
 
    
    condwait.signal(&condwait);
    

}




stPlayList *fileplaylist = NULL;
stPlayList *headoflist = NULL;
stPlayList *selectedlist = NULL;

#define NOOFTHREADLOAD 0
int main(int argc, char**argv) 
{
 #if 1
    readplaylist(&fileplaylist);
    
    selectedlist = fileplaylist;
    
    swapplayNode( fileplaylist, true, &selectedlist);
    
    printf("\n selected node %s \n", selectedlist->file );
    
    displaylist(fileplaylist);
            
    swapplayNode( fileplaylist->next, false, &selectedlist);
    
    printf("\n selected node %s \n", selectedlist->file ) ;     
    
    displaylist(fileplaylist);
    
    
    removeplayNode( &fileplaylist, fileplaylist, &headoflist);
    
    displaylist(fileplaylist);
    
    
    getPage( fileplaylist , true,  &headoflist);
    
    removeplayNode( &fileplaylist, fileplaylist->next, &headoflist);
    
    displayPage(headoflist);
     
    getPage( fileplaylist , true,  &headoflist);
    
    displayPage(headoflist);
    
    getPage( fileplaylist , true,  &headoflist);
    
    displayPage(headoflist);
     
    getPage( fileplaylist , true,  &headoflist);
     
    
    displayPage(headoflist);
        
    //headoflist = NULL;
    
    getPage( fileplaylist , false,  &headoflist);
             
    displayPage(headoflist);
    
    getPage( fileplaylist , false,  &headoflist);
             
    displayPage(headoflist);
    
    
    getPage( fileplaylist , false,  &headoflist);
             
    displayPage(headoflist);
    
    
    
    getPage( fileplaylist , false,  &headoflist);
             
    displayPage(headoflist);
    
    
    
//    
//    addplaylist_front( &fileplaylist, "arvind" );
//    
//    addplaylist_front( &fileplaylist, "arvind1" );
//    
//    addplaylist_front( &fileplaylist, "arvind2" );
//      
//    addplaylist_front( &fileplaylist, "arvind3" );
//    
//    removeplaylist( &fileplaylist, "arvind2" );
//    
//     saveplaylist(fileplaylist);
//    displaylist(fileplaylist);
//    
//    addplaylist_front( &fileplaylist, "A" );
//    addplaylist_front( &fileplaylist, "B" );
//    addplaylist_front( &fileplaylist, "C" );
//   removeplaylist( &fileplaylist, "D" );    
//      
    
  
    saveplaylist(fileplaylist);
   // displaylist(fileplaylist);
    
    fflush(stdout);
#endif
    
#if 0
    signal(SIGINT, intHandler); 
    signal(SIGTSTP, intHandler); 
            
    condwait.init(&condwait);
        
    Slogger slogger = (Slogger){   slog_start, slog_stop, "/tmp/GPU001.log" }; 
    slogger.start(&slogger); 
    
       
    slog_message(LOG_INFO, TAG, "GPU001 started");
    
 

    
    
//    char* arg_list1[] = {  "ls",    "-l",  "/",     NULL      };
//    
//    Exec thexecList = (Exec){ exec_start, exec_run, exec_run,  "/tmp/", "parameter1.txt", arg_list1, true}; 
//    thexecList.start(&thexecList); 
//    thexecList.stop(&thexecList); 
//    
    
    //exec_top(); // mpstat -P ALL  1
      
    //char* arg_list[] = {  "top",  "-d",   "1",   NULL  };
    char* arg_list[] = {  "mpstat",  "-P",   "ALL", "1",  NULL  };
    Exec thexec = (Exec){ exec_start, exec_run, exec_stop,  "/tmp/", "parameter.txt", arg_list}; 
    thexec.start(&thexec); 
    
    
    condwait.wait(&condwait, 5, 10);
    
    
    thexec.stop(&thexec); 
    
    slog_message(LOG_INFO,  TAG, "GPU001 ended");
    
    slogger.stop(&slogger);
    
    condwait.stop(&condwait);
    
    
    
#else
    
    signal(SIGINT, intHandler); 
    signal(SIGTSTP, intHandler); 
            
    condwait.init(&condwait);

    Slogger slogger = (Slogger){   slog_start, slog_stop, "/tmp/GPU001.log" }; 
    slogger.start(&slogger); 
    
       
    slog_message(LOG_INFO, TAG, "GPU001 started");
    
    slog_message(LOG_INFO, TAG, "Gear and lanes are configured and validated");

 
//    ThLoader tloader = (ThLoader){ thload_start, thload_run, thload_stop, 0, "/tmp/", "parameter.txt"  }; 
    
//    tloader.start(&tloader); 
    
    ThLoader tloaderArr[NOOFTHREADLOAD];
    
    for(int n=0 ; n < NOOFTHREADLOAD ; ++n )
    {
        tloaderArr[n] = (ThLoader){ thload_start, thload_run, thload_stop, 1 , "/tmp/"  }; 
        tloaderArr[n].start(&tloaderArr[n]); 
    }
    
    condwait.wait(&condwait, 5, 10);
    
    
    for(int n=0 ; n < NOOFTHREADLOAD ; ++n )
    {
        tloaderArr[n].stop(&tloaderArr[n]); 
    }
    
//    tloader.stop(&tloader); 
    
    slog_message(LOG_INFO,  TAG, "GPU001 ended");
    
    slogger.stop(&slogger);
    
    condwait.stop(&condwait);
#endif
        
    printf("\nmain exit\n");
    
    return 0;
}
