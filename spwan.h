#ifndef EXEC_H
#define EXEC_H

#include <pthread.h>
#include <stdatomic.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _execload { 

  void (*start)( struct _execload*);
  void (*run)( struct _execload*); 
  void (*stop)( struct _execload*); 
  
  char logPath[MAX_PATH_LENGTH]; //for eg /tmp/
  char logfile[MAX_PATH_LENGTH]; //UFS001.txt  
  
  char **argv;
  
  bool blocking;
   
  atomic_bool keeprunning; 
  pthread_t threads; 
  
  FILE *thread_log;
  
  FILE *fp;
  
  int pid;
  
} Exec ; 

int exec(char* arg_list[], char **out, int *retSize  );
int exec_log(char* arg_list[] );


void exec_start(Exec* th);

void exec_run(Exec* th);

void exec_stop(Exec* th);



#ifdef __cplusplus
}
#endif

#endif  /* EXEC_H */


