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




#define TAG "UFS : Benchmark"


//https://github.com/H-jamil/transferService/blob/main/sender.c

//https://github.com/raysan5/raylib/blob/master/examples/core/core_loading_thread.c

//while (atomic_load_explicit(&flag, SOME_MEMORY_ORDER)) ...


//#define CHUNK_SIZE 1048576 // 1 MB
//#define FILE_PATH "/tmp/FILE0.txt"





//


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


int main(int argc, char**argv) 
{


    
    
    signal(SIGINT, intHandler); 
    
    printf("\nmain\n");
        
    

    Slogger slogger = (Slogger){  NULL, slog_start, slog_stop, "/tmp/test.log" }; 
    slogger.start(&slogger); 
    
    
       
    log_message(LOG_DEBUG, NULL, TAG, "UFS001 started");
    
    log_message(LOG_DEBUG, NULL, TAG, "Gear and lanes are configured and validated");
    
 
 
    ThLoader tloader = (ThLoader){ thload_start, thload_run, thload_stop, "/tmp/"  }; 
    tloader.start(&tloader); 
    
    
    
    condwait.wait(&condwait, 25, 1);
    
    tloader.stop(&tloader); 

    
    
    log_message(LOG_DEBUG, NULL, TAG, "UFS001 ended");
    
    
    slogger.stop(&slogger);
    
    condwait.stop(&condwait);
    
        
    printf("\nmain exit\n");
    
    return 0;
}
