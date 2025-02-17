/*
 * FileName:      condwait.c
 * Description:   Implements functions for condition variable synchronization, 
 *                including `condwait_wait` for waiting with a timeout, 
 *                `condwait_signal` for signaling, and `condwait_stop` for cleanup.
  * Author:       Arvind Umrao <aumrao@google.com> 
 *                Rajanee Kumbhar <rajaneek@google.com>
 */

#include "condwait.h"
#include "common.h"

void condwait_int(Condwait* th)
{
    pthread_mutex_init(&th->mutex, NULL);
    pthread_cond_init(&th->cond, NULL);
    
    clock_gettime(CLOCK_REALTIME, &th->timeout);
    
}


/**
 * Function: int condwait_wait(Condwait* th, int timeInSec, int timeInMs)
 * Description: Waits for a condition variable to be signaled, with a specified 
 *              timeout (in seconds and milliseconds). Returns 0 if signaled, 
 *              or ETIMEDOUT if the timeout occurs.
 * Parameters:
 *   - th: Condwait* - Pointer to a `Condwait` structure containing the mutex 
 *     and condition variable.
 *   - timeInSec: int - Timeout duration in seconds.
 *   - timeInMs: int - Timeout duration in milliseconds.
 * Returns:
 *   int - 0 if the condition was signaled, or ETIMEDOUT if the timeout occurred.
 */
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
       // printf("Condition signaled.\n");
    } else if (result == ETIMEDOUT) {
        //printf("Timeout occurred.\n");
    }
    
    //pthread_attr_destroy(&attr);
    return result;
} 
 
/**
 * Function: void condwait_signal(Condwait* th)
 * Description: Signals a condition variable to wake up a waiting thread. 
 *              It unlocks the mutex before signaling and then re-locks it.
 * Parameters:
 *   - th: Condwait* - Pointer to a `Condwait` structure containing the mutex 
 *     and condition variable.
 * Returns:
 *   void - No return value.
 */
void condwait_signal(Condwait* th){ 
    pthread_mutex_unlock(&th->mutex);
    pthread_cond_signal(&th->cond);
    pthread_mutex_unlock(&th->mutex);
} 

/**
 * Function: void condwait_stop(Condwait* th)
 * Description: Cleans up by destroying the condition variable and unlocking 
 *              the associated mutex.
 * Parameters:
 *   - th: Condwait* - Pointer to a `Condwait` structure containing the mutex 
 *     and condition variable.
 * Returns:
 *   void - No return value.
 */
void condwait_stop(Condwait* th){ 
    
    pthread_mutex_lock(&th->mutex);
    pthread_cond_destroy(&th->cond);
    pthread_mutex_unlock(&th->mutex);
} 
