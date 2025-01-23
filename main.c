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

#define TAG "UFS : Frequency"


//https://github.com/H-jamil/transferService/blob/main/sender.c

//https://github.com/raysan5/raylib/blob/master/examples/core/core_loading_thread.c

//while (atomic_load_explicit(&flag, SOME_MEMORY_ORDER)) ...


//#define CHUNK_SIZE 1048576 // 1 MB
//#define FILE_PATH "/tmp/FILE0.txt"






//void cancel_my_thread(void) {
//    atomic_store_explicit(&keeprunning,0 , memory_order_relaxed);
//}

//
void intHandler(int sig)
{ // can be called asynchronously
 
    
//    printf("intHandler\n" );
//    
//    queue.shutdown = 1;
//    
//    cancel_my_thread();
//
//    pthread_cond_signal(&queue.cond);
    
    fflush(stdout);
  
}



struct manifiest
{
    char name[15];
    int totalThread;
    

};//end of struct student


struct threadid_fd
{
    pid_t tidid;
    long int FDPos;

};//end of struct student


void readT() 
{
    struct threadid_fd st;
    FILE *fptr;
    fptr = fopen("/tmp/manifest.txt", "r");
    if (fptr != NULL) {
        while (fread(&st, sizeof (st), 1, fptr)) {
            printf("\n tid is : %ld", st.tidid);
            printf("\n FDPos is : %ld", st.FDPos);

            printf("\n********************************************");
        }
    } else {
        printf("\nCould not open file");
    }
    fclose(fptr); //closing file
}//end 


void writeT( int threadNO ) 
{
    struct threadid_fd st;
    FILE *fptr;
    fptr = fopen("/tmp/manifest.txt", "w");
    if (fptr != NULL) {
        

        st.tidid = 101;
        st.FDPos = 300;
        
        printf("\n tid is :%ld", st.tidid);
        printf("\n FDPos is :%ld", st.FDPos);

        printf("\n********************************************");

        for( int x=0; x < threadNO ; ++x  )
         fwrite(&st, sizeof (st), 1, fptr);
       
    } else {
        printf("\nCould not open file");
    }
    fclose(fptr); //closing file
}//end 



int readM() 
{
    struct manifiest st;
    FILE *fptr;
    fptr = fopen("/tmp/manifest.txt", "r");
    if (fptr != NULL) {
        while (fread(&st, sizeof (st), 1, fptr)) {
            printf("\n Name is :%s", st.name);
            printf("\n totalThread is :%d", st.totalThread);
            printf("\n********************************************");
        }
    } else {
        printf("\nCould not open file");
    }
    fclose(fptr); //closing file
    
    return st.totalThread;
    
}//end 


int  writeM() 
{
    struct manifiest st;
    FILE *fptr;
    fptr = fopen("/tmp/manifest.txt", "w");
    if (fptr != NULL) {
        
        strcpy( st.name, "avind");
        st.totalThread = 30;
        
        printf("\n Name is :%s", st.name);
        printf("\n totalThread is :%d", st.totalThread);


        fwrite(&st, sizeof (st), 1, fptr);
       
    } else {
        printf("\nCould not open file");
    }
    
    
    fclose(fptr); //closing file
    
    
}//




//int fillQueue()
//{
//
//
////    // get size of file
////    FILE* file = fopen(FILE_PATH, "rb");
////    if (file == NULL) {
////        fprintf(stderr, "Failed to open file\n");
////        return 1;
////    }
////    fseek(file, 0L, SEEK_END);
////    long file_size = ftell(file);
////    fclose(file);
//
//        // create ZeroMQ context
////    void* context = zmq_ctx_new();
//
//    // create worker threads
//    pthread_t threads;
//    pthread_create(&threads, NULL, logging_th, NULL);
//
//
//    // initialize queue
//    queue.capacity = CAPACITY;
//    queue.data = malloc(queue.capacity * sizeof(QueueData*));
//   
//
//    logit("arvind");
//            
//    logit("ravind");
//  
//
//    // wait for worker threads to finish
//
//    pthread_join(threads, NULL);
//
//    // send end message
//
//    /*
//    void* socket = zmq_socket(context, ZMQ_PUSH);
//    zmq_connect(socket, endpoint);
//    * */
////    
////    char end_msg[CHUNK_SIZE] = "end";
////    memset(end_msg + 3, 0, CHUNK_SIZE - 3); // fill the rest of the message with zeroes
////    zmq_send(socket, end_msg, CHUNK_SIZE, 0);
////    printf("Sent end message\n");
//
//    // clean up
//
//    //free(tasks);
//    free(queue.data);
//
//
//    return 0;
//}


void logger(FILE *fp,  const char* message )
{
    pthread_t tid = pthread_self();
    
    char store[516];
    printf(fp, "%s %s %s %d \n", "__FILE__", "__LINE__", message , tid);
    sprintf(store, "%s %s %s %d \n", "__FILE__", "__LINE__", message , tid);
    
}


void logit(  const char* message )
{
    pthread_t tid = pthread_self();
    
    char store[516];
    
    sprintf(store, "%s %s %s %d \n", "__FILE__", "__LINE__", message , tid);
    
    pushMessage(store , strlen(store) );
    
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
//    Node  ll = (Node){ 42, NULL, str }; 
//    ll.str_func(&ll); 
//    printf("Hello World"); 
//  
    
    signal(SIGINT, intHandler); 
    
    printf("\nmain\n");
        
    

    Slogger slogger = (Slogger){  NULL, slog_start, slog_stop, " " }; 
    
    slogger.start(&slogger); 
    
    
       
    log_message(LOG_DEBUG, NULL, TAG, "UFS001 started");
    
    log_message(LOG_DEBUG, NULL, TAG, "Gear and lanes are configured and validated");
    
 
    sleep(20);
    
    log_message(LOG_DEBUG, NULL, TAG, "UFS001 ended");
    
    
    slogger.stop(&slogger);
    
    
    return 0;
}
