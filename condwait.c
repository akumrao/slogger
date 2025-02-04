
#include "condwait.h"
#include "common.h"

int condwait_wait(Condwait* th, int timeInSec, int timeInMs){ 
    

    struct timespec timeout;
    
//    pthread_condattr_t attr;
//    pthread_condattr_init( &attr);
//    pthread_condattr_setclock( &attr, CLOCK_MONOTONIC);
   
//    if (pthread_cond_init(&th->cond, NULL) != 0) {                                    
//        perror("pthread_cond_init() error");                                        
//        exit(1);                                                                    
//    }   

    // Initialize mutex and condition variable
    
    pthread_mutex_init(&th->mutex, NULL);
    pthread_cond_init(&th->cond, NULL);
    

    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += timeInSec; // Timeout of seconds
    timeout.tv_nsec += (1000UL*timeInMs)*1000UL; // Timeout in millsecs

    pthread_mutex_lock(&th->mutex);
     
    int result = pthread_cond_timedwait(&th->cond, &th->mutex, &timeout);

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