
#include "slogger.h"
#include "common.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <string.h>
#include <signal.h> //  our new library 

#include <stdatomic.h>


#define CAPACITY  2048


/*
 * For logger thread to keep running
 */

static atomic_bool keeprunning = 1;  


/*
 * Structure for Queue Data
 */


typedef struct {
    char *str;
    long size;
} QueueData;

typedef struct {
    
    int front, size; 
    //int size;
    
    int shutdown;
    QueueData** data;
    size_t capacity;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;

Queue queue = {
    .front = 0,
    .size = 0,
    .shutdown = 0,
    .data = NULL,
    .capacity = 0,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER
};

void enqueue(QueueData* data) {
    pthread_mutex_lock(&queue.mutex);
    if (queue.size == queue.capacity || queue.shutdown) {
       //pthread_cond_wait(&queue.cond, &queue.mutex);
        
        pthread_mutex_unlock(&queue.mutex);
        
        if(queue.size == queue.capacity)
        fprintf(stderr, "Logger buffer overflow. \n"); // This condition will not hit until test code is wrong.
        
        return; 
    }
    
    int rear = (queue.front + queue.size) % queue.capacity;
    
    queue.data[rear] = data; 
    queue.size++;

    pthread_cond_signal(&queue.cond);
    pthread_mutex_unlock(&queue.mutex);
}

QueueData* dequeue() {
    pthread_mutex_lock(&queue.mutex);
    while (queue.size == 0 && !queue.shutdown  ) {
        pthread_cond_wait(&queue.cond, &queue.mutex);
    }
    if( queue.size == 0 && queue.shutdown )
    {
        pthread_mutex_unlock(&queue.mutex);
        return NULL; 
    }
    
    QueueData* data = queue.data[queue.front];
    queue.front = (queue.front + 1) % queue.capacity;
    queue.size--;

   // pthread_cond_signal(&queue.cond);
    pthread_mutex_unlock(&queue.mutex);
    return data;
}


//while (atomic_load_explicit(&flag, SOME_MEMORY_ORDER)) ...

void* logging_thread(void* arg) {

    
    const char *logPath = (const char *) arg;
    
   // printf(" \n test %s \n", logPath);
    
    
        // open file
    FILE* file = fopen(logPath, "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return NULL;
    }
    
    printf("logging thread started\n");

    while (1)
    {
        // get next chunk from queue
        QueueData* qdata = dequeue();
        if(!qdata)
            break;
        
        
        fwrite(qdata->str, 1, qdata->size, file);
         
        fflush(file); 
                    
       // printf("qdata: %s, size: %d \n", qdata->str,  qdata->size);
        
        free(qdata->str);
        free(qdata);
        
       // fflush(stdout);

//        // seek to start of chunk
//        if (fseek(file, data->start, SEEK_SET) != 0) {
//            fprintf(stderr, "Failed to seek file\n");
//            fclose(file);
//            return NULL;
//        }

        // read chunk
        //size_t size = data->end - data->start;
//        char* buffer = malloc(size);
//        if (fread(buffer, 1, size, file) != size) {
//            fprintf(stderr, "Failed to read file\n");
//            fclose(file);
//            free(buffer);
//            return NULL;
//        }

        // close file
       

        // send chunk over socket
//        if (zmq_send(data->socket, buffer, size, 0) < 0) {
//            fprintf(stderr, "Failed to send data\n");
//        }

        // clean up
        //free(buffer);
    }
    
    fclose(file);
     
    printf("End of logging_thread \n");
    
    fflush(stdout);
    
    return NULL;
}




void pushMessage(const char *str , long size )
{
    QueueData* tasks = malloc( sizeof(QueueData));
    if(tasks)
    {
        // create task data
        //tasks->str = malloc(size);
        if(str )
        {
            //memcpy( tasks->str , str, size);
            tasks->str = str;
            tasks->size = size;
            // enqueue task
            enqueue(tasks);
        }
        else
             fprintf(stderr, "%s", "Out of Memory \n");
    }
    else
        fprintf(stderr, "%s", "Out of Memory \n");
    
}




void slog_start(Slogger* th){ 
    
   // pthread_t threads;
    pthread_create(&th->threads, NULL, logging_thread, th->logFile);

    // initialize queue
    queue.capacity = CAPACITY;
    queue.data = malloc(queue.capacity * sizeof(QueueData*));
    
} 
 

void slog_stop(Slogger* th){ 
    
     
    printf("slog_stop\n" );
    
    queue.shutdown = 1;
    
    //atomic_store_explicit(&keeprunning,0 , memory_order_relaxed);

    pthread_mutex_lock(&queue.mutex);
    
    pthread_cond_signal(&queue.cond);
    pthread_cond_destroy(&queue.cond);
    
    pthread_mutex_unlock(&queue.mutex);

    
    fflush(stdout);
    
    
    pthread_join(th->threads, NULL);
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

    free(queue.data);
   
} 
