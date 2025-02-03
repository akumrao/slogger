
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
    if(queue.shutdown )
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
    
    printf(" \n test %s \n", logPath);
    
    
        // open file
    FILE* file = fopen("/tmp/manifest.txt", "w+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return NULL;
    }
    
    printf("logging thread started\n");

    while (atomic_load_explicit(&keeprunning, memory_order_relaxed))
    {
        // get next chunk from queue
        QueueData* qdata = dequeue();
        if(!qdata)
            break;
        
        
        fwrite(qdata->str, 1, qdata->size, file);
         
        fflush(file); 
                    
        printf("qdata: %s, size: %d \n", qdata->str,  qdata->size);
        
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
        tasks->str = malloc(size);
        if(tasks->str )
        {
            memcpy( tasks->str , str, size);
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


/**
 * Function: void log_message(int log_lvl, FILE *fp,
 *                             const char *tag, const char *fmt, ...)
 * Description: Logs a message to the console and/or a
 *              file, depending on the log level, log
 *              type, and file pointer. It formats the
 *              message with a timestamp and tag.
 * Parameters:
 *   - log_lvl: int - The log level of the message
 *     (e.g., LOG_INFO, LOG_ERR, LOG_DEBUG).
 *   - fp: FILE * - A file pointer to log to a file,
 *     or NULL to log only to the console.
 *   - tag: const char * - A tag to identify the
 *     source of the log message.
 *   - fmt: const char * - The format string for the
 *     log message.
 *   - ...: Variable arguments to be formatted
 *     according to the format string.
 * Returns:
 *   void - This function does not return a value.
 */
void log_message(int log_lvl, FILE *fp, const char *tag, const char *fmt, ...)
{
  //if (log_lvl <= console_loglevel)
  {
    char timestamp[LOG_BUFFER_SIZE];
    get_timestamp(timestamp, sizeof(timestamp));

    va_list args;
//    va_start(args, fmt);
//    switch (log_type) {
//      case LOG_TYPE_LOGCAT:
//      case LOG_TYPE_ADBSHELL:
//        switch (log_lvl) {
//          case LOG_INFO:
//            printf(COLOR_BLUE "%s  INFO %s: ", timestamp, tag); VAR_LOG; break;
//          case LOG_ERR:
//            printf(COLOR_RED "%s ERROR %s: ", timestamp, tag); VAR_LOG; break;
//          case LOG_DEBUG:
//            printf(COLOR_YELLOW "%s DEBUG %s: ", timestamp, tag); VAR_LOG; break;
//          default:
//            printf(COLOR_GREEN "%s  WARN %s: ", timestamp, tag); VAR_LOG; break;
//        }
//        break;
//      case LOG_TYPE_SERIAL:
//        break;
//      default:
//        break;
//    }
//    va_end(args);

    
    char store[2048];
    
    //sprintf(store, "%s %s %s %d \n", "__FILE__", "__LINE__", message , tid);
    int ncount = 0;
    
    /* Log to file if the file pointer is not NULL */
    //if (fp != NULL) 
    {
      switch (log_lvl) {
        case LOG_INFO:
            ncount += sprintf(&store[ncount], "%s  INFO %s:  ", timestamp, tag); break;
        case LOG_ERR:
           ncount += sprintf(&store[ncount], "%s ERROR %s:  ", timestamp, tag); break;
        case LOG_DEBUG:
           ncount += sprintf(&store[ncount], "%s DEBUG %s:  ", timestamp, tag); break;
        default:
          sprintf(&store[ncount], "%s  WARN %s:  ", timestamp, tag); break;
      }
      va_start(args, fmt); /* Restart the variable argument list */
         ncount += vsprintf(&store[ncount], fmt, args);  ncount += sprintf(&store[ncount], "\n"); fflush(fp);
      va_end(args);
    }
    
    printf(store);
    pushMessage(store , strlen(store) );
    
    
    
  }
}

void slog_start(Slogger* th){ 
    
   // pthread_t threads;
    pthread_create(&th->threads, NULL, logging_thread, th->logPath);

    // initialize queue
    queue.capacity = CAPACITY;
    queue.data = malloc(queue.capacity * sizeof(QueueData*));
    
    
} 
 

void slog_stop(Slogger* th){ 
    
     
    printf("slog_stop\n" );
    
    queue.shutdown = 1;
    
    atomic_store_explicit(&keeprunning,0 , memory_order_relaxed);

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
