#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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


#define TAG "GPU : Benchmark"



 Condwait condwait = (Condwait){  condwait_wait, condwait_signal, condwait_stop }; 
 

void intHandler(int sig)
{ // can be called asynchronously
 
    
    condwait.signal(&condwait);
    
    printf("intHandler\n" );
//    
//    queue.shutdown = 1;
//    
//    cancel_my_thread();
//
//    pthread_cond_signal(&queue.cond);
    
    
    
    fflush(stdout);
  
}




typedef struct _node { 
   
  int value; 
  struct _node *next; 
   
  void (*str_func)( struct _node*); 
     
} Node ; 
 
void str(Node* n){ 
    while ( n!= NULL ){ 
        printf("%d -> ", n->value); 
        n = n->next; 
    } 
    printf("NULL\n"); 
} 
 

/*
void* myThreadFun(void* vargp)
{
    sleep(1);
    printf("Printing GeeksQuiz from Thread \n");
    return NULL;
}

int main()
{
    pthread_t thread_id;
    printf("Before Thread\n");
    pthread_create(&thread_id, NULL, myThreadFun, NULL);
    pthread_join(thread_id, NULL);
    printf("After Thread\n");
    exit(0);
}*/

#define NOOFTHREADLOAD 50
int main(int argc, char**argv) 
{
    
    signal(SIGINT, intHandler); 
    signal(SIGTSTP, intHandler); 
            

    Slogger slogger = (Slogger){   slog_start, slog_stop, "/tmp/GPU001.log" }; 
    slogger.start(&slogger); 
    
    
       
    slog_message(LOG_DEBUG, TAG, "UFS001 started");
    
    slog_message(LOG_DEBUG, TAG, "Gear and lanes are configured and validated");
    
 
   // ThLoader tloader = (ThLoader){ thload_start, thload_run, thload_stop, 0, "/tmp/", "parameter.txt  }; 
    
   // tloader.start(&tloader); 
    
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
    
    
    slog_message(LOG_DEBUG,  TAG, "UFS001 ended");
    
    slogger.stop(&slogger);
    
    condwait.stop(&condwait);
    
        
    printf("\nmain exit\n");
    
    return 0;
}
