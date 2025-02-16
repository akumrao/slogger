
#include "condwait.h"
#include "common.h"

void condwait_int(Condwait* th)
{
    pthread_mutex_init(&th->mutex, NULL);
    pthread_cond_init(&th->cond, NULL);
    
    clock_gettime(CLOCK_REALTIME, &th->timeout);
    
}

int condwait_wait(Condwait* th, int timeInSec, int timeInMs){ 
    

    
    
//    pthread_condattr_t attr;
//    pthread_condattr_init( &attr);
//    pthread_condattr_setclock( &attr, CLOCK_MONOTONIC);
   
//    if (pthread_cond_init(&th->cond, NULL) != 0) {                                    
//        perror("pthread_cond_init() error");                                        
//        exit(1);                                                                    
//    }   

    // Initialize mutex and condition variable
    
    u_int64_t future_ns = th->timeout.tv_nsec + timeInMs * 1000000L;
    th->timeout.tv_nsec = future_ns % 1000000000;
    th->timeout.tv_sec += timeInSec + future_ns / 1000000000; // Timeout of seconds
   
    pthread_mutex_lock(&th->mutex);
     
    int result = pthread_cond_timedwait(&th->cond, &th->mutex, &th->timeout);

    pthread_mutex_unlock(&th->mutex);

    if (result == 0) {
        printf("Condition signaled.\n");
    } else if (result == ETIMEDOUT) {
        printf("Timeout occurred.\n");
    }
    
    //pthread_attr_destroy(&attr);
    return result;
    
} 
 

void condwait_signal(Condwait* th){ 
    
    pthread_mutex_unlock(&th->mutex);
   
    pthread_cond_signal(&th->cond);
    
    pthread_mutex_unlock(&th->mutex);
   
} 


void condwait_stop(Condwait* th){ 
    
    pthread_mutex_lock(&th->mutex);

    pthread_cond_destroy(&th->cond);
    
    pthread_mutex_unlock(&th->mutex);
   
} 
