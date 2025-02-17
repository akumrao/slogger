/*
 * Copyright (c) 2024-25 Google SLT Team
 *
 * FileName:      threadload.c
 * Description:   File have common code wrapper implementation to gpu.
 * Author:        Arvind Umrao <aumrao@google.com> 
 *                
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h> 

#include "common.h"

#include "spwan.h"

#define TAG "TOP "

#define READ   0
#define WRITE  1



FILE * fork_dup2_exec(char* cArgv[], char type, int *pid) {
	pid_t child_pid;
	int fd[2];
	pipe(fd);

	if((child_pid = fork()) == -1) {
		perror("fork");
		exit(1);
	}


	/* child process */
	if (child_pid == 0) {

		//Don't inherit parent signal handlers
		signal(SIGINT,  SIG_IGN);
		signal(SIGTERM, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);

		if (type == 'r') {
			close(fd[READ]);    //Close the READ end of the pipe since the child's fd is write-only
			dup2(fd[WRITE], 1); //Redirect stdout to pipe
		}
		else {
			close(fd[WRITE]);    //Close the WRITE end of the pipe since the child's fd is read-only
			dup2(fd[READ], 0);   //Redirect stdin to pipe
		}

		
		if(execvp(cArgv[0], cArgv) == -1) {
			perror("execvp failed: ");
			_Exit(EXIT_FAILURE);
		}
		_Exit(EXIT_SUCCESS);
	}
	else {
		if (type == 'r') {
			close(fd[WRITE]); //Close the WRITE end of the pipe since parent's fd is read-only
		}
		else {
			close(fd[READ]); //Close the READ end of the pipe since parent's fd is write-only
		}
	}

	*pid = child_pid;

	if (type == 'r')
        {
		return fdopen(fd[READ], "r");
	}

	return fdopen(fd[WRITE], "w");
}

int fork_dup2_exec_result(FILE * fp, pid_t child_pid) {
	int status;
	pid_t wpid;
	fclose(fp);
	//while ((wpid = waitpid(pid, &status, 0)) == -1) {
	do {
		wpid = waitpid(child_pid, &status, WUNTRACED
#ifdef WCONTINUED       /* Not all implementations support this */
				| WCONTINUED
#endif
			      );
		/*
		if (errno != EINTR) {
			stat = -1;
			break;
		}
		*/
		if(wpid == -1) {
			perror("error waitpid call");
			return -1;
		}
		if (WIFEXITED(status)) {
			printf("child exited, status=%d\n", WEXITSTATUS(status));

		} else if (WIFSIGNALED(status)) {
			printf("child killed (signal %d)\n", WTERMSIG(status));

		} else if (WIFSTOPPED(status)) {
			printf("child stopped (signal %d)\n", WSTOPSIG(status));
                        kill(child_pid, SIGCONT); 

#ifdef WIFCONTINUED     /* Not all implementations support this */
		} else if (WIFCONTINUED(status)) {
			printf("child continued\n");
#endif
		} else {    /* Non-standard case -- may never happen */
			printf("Unexpected status (0x%x)\n", status);
		}

	} while (!WIFEXITED(status) && !WIFSIGNALED(status));

	return status;
}

#define USE_FORK_DUP2_EXEC

#ifdef USE_FORK_DUP2_EXEC
int exec(char* argv[] ,  char **out, int* outsize   ) {
	int pid;

	FILE * fp = fork_dup2_exec(argv, 'r', &pid);
	char command_out[100] = {0};

        int sz = 0;
        
        *out = NULL;
        *outsize = 0;
        
	while ( (sz = read(fileno(fp), command_out, sizeof(command_out)-1)) != 0)
	{   

            char *tmp = (char *)realloc(*out, *outsize + sz );
            if(tmp)
            {
               memcpy( &tmp[ *outsize], command_out, sz );
               
               *out = tmp;
            }

            *outsize += sz;
            
	}
        
	if(fork_dup2_exec_result(fp, pid) != 0) {
		return -1;
	}
	return *outsize;

}

void* load_exec(void * arg) {

    
    Exec *th = (Exec *) arg;
    
    //printf(" \n test %s \n", th->logPath);
    
    char thread_log_filename[256];
   
    sprintf (thread_log_filename, "%s%s", th->logPath, th->logfile); 

    th->thread_log = fopen (thread_log_filename, "w+");
    if (th->thread_log == NULL) {
          fprintf(stderr, "Failed to open file\n");
          return NULL;
    }
  

    th->run(th);


    slog_message(LOG_DEBUG, TAG, "End of thread %s ",  (long) pthread_self () );
      
    fclose (th->thread_log);
        
    
    return NULL;
}



void exec_start(Exec* th){ 
    
    
    th->fp = fork_dup2_exec(th->argv, 'r', &th->pid);
      
   // pthread_t threads;
    th->keeprunning = 1;
  //  pthread_key_create (&thread_log_key, close_thread_log);  // this line make the thread slow, please do not enable it
    pthread_create(&th->threads, NULL, load_exec, th);
    
} 

void exec_run(Exec* th){ 
    
    
    char command_out[100] = {0};
    int sz = 0;

    while ( atomic_load_explicit(&th->keeprunning, memory_order_relaxed)  &&  ((sz = read(fileno(th->fp), command_out, sizeof(command_out)-1)) != 0  ))
    {   
       // printf("cmd %s\n", command_out);
       //fprintf(th->thread_log, "%s", command_out);
       fwrite(command_out, 1, sz, th->thread_log);

    }

      
    return ;
} 



void exec_stop(Exec* th){ 
    
     
    printf("exec_stop\n" );
    
    if(!th->blocking )
    {
        atomic_store_explicit(&th->keeprunning,0 , memory_order_relaxed);

        kill(th->pid, SIGINT);
    }

    if(fork_dup2_exec_result(th->fp, th->pid) != 0) {
            return;
    }

    pthread_join(th->threads, NULL);

    printf("exec_joined\n" );
} 





#else
int exec(const std::string &in_cmd, std::string &out) {
     FILE *stream =  popen(in_cmd.c_str(), "r");;
     if (!stream) return -1;
     char buffer[128];
     out = "";
     while (!feof(stream))
     {
          if (fgets(buffer, 128, stream) != NULL)
               out += buffer;
     }
     pclose(stream);
     return 0;
}
#endif



#if 0

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
//https://github.com/atkunal/stack-smashing/blob/master/Forking%2C%20Part%202:%20Fork%2C%20Exec%2C%20Wait%20Kill.md
static volatile sig_atomic_t done = 0;
https://gist.github.com/er0p/8dcdaa0d738704807c6243a1ea5dc6bd
https://gist.github.com/er0p/8dcdaa0d738704807c6243a1ea5dc6bd
https://web.stanford.edu/class/cs110/summer-2021/lecture-notes/lecture-08/
static void handle_done(int signum)
{
    if (!done)
        done = signum;
}

static int install_done(const int signum)
{
    struct sigaction act;

    memset(&act, 0, sizeof act);
    sigemptyset(&act.sa_mask);
    act.sa_handler = handle_done;
    act.sa_flags = 0;
    if (sigaction(signum, &act, NULL) == -1)
        return errno;

    return 0;
}

static int  deathfd = -1;

static void death(int signum, siginfo_t *info, void *context)
{
    if (info->si_code == POLL_IN && info->si_fd == deathfd)
        raise(SIGTERM);
}

static int install_death(const int signum)
{
    struct sigaction act;

    memset(&act, 0, sizeof act);
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = death;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(signum, &act, NULL) == -1)
        return errno;

    return 0;
}

int main(void)
{
    pid_t  child, p;
    int    pipefd[2], status;
    char   buffer[8];

    if (install_done(SIGINT)) {
        fprintf(stderr, "Cannot set SIGINT handler: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Cannot create control pipe: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    child = fork();
    if (child == (pid_t)-1) {
        fprintf(stderr, "Cannot fork child process: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (!child) {
        /*
         * Child process.
        */

        /* Close write end of pipe. */
        deathfd = pipefd[0];
        close(pipefd[1]);

        /* Set a SIGHUP signal handler. */
        if (install_death(SIGHUP)) {
            fprintf(stderr, "Child process: cannot set SIGHUP handler: %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }

        /* Set SIGTERM signal handler. */
        if (install_done(SIGTERM)) {
            fprintf(stderr, "Child process: cannot set SIGTERM handler: %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }

        /* We want a SIGHUP instead of SIGIO. */
        fcntl(deathfd, F_SETSIG, SIGHUP);

        /* We want the SIGHUP delivered when deathfd closes. */
        fcntl(deathfd, F_SETOWN, getpid());

        /* Make the deathfd (read end of pipe) nonblocking and async. */
        fcntl(deathfd, F_SETFL, O_NONBLOCK | O_ASYNC);

        /* Check if the parent process is dead. */
        if (read(deathfd, buffer, sizeof buffer) == 0) {
            printf("Child process (%ld): Parent process is already dead.\n", (long)getpid());
            return EXIT_FAILURE;
        }

        while (1) {
            status = __atomic_fetch_and(&done, 0, __ATOMIC_SEQ_CST);
            if (status == SIGINT)
                printf("Child process (%ld): SIGINT caught and ignored.\n", (long)getpid());
            else
            if (status)
                break;
            printf("Child process (%ld): Tick.\n", (long)getpid());
            fflush(stdout);
            sleep(1);

            status = __atomic_fetch_and(&done, 0, __ATOMIC_SEQ_CST);
            if (status == SIGINT)
                printf("Child process (%ld): SIGINT caught and ignored.\n", (long)getpid());
            else
            if (status)
                break;
            printf("Child process (%ld): Tock.\n", (long)getpid());
            fflush(stdout);
            sleep(1);
        }

        printf("Child process (%ld): Exited due to %s.\n", (long)getpid(),
               (status == SIGINT) ? "SIGINT" :
               (status == SIGHUP) ? "SIGHUP" :
               (status == SIGTERM) ? "SIGTERM" : "Unknown signal.\n");
        fflush(stdout);

        return EXIT_SUCCESS;
    }

    /*
     * Parent process.
    */

    /* Close read end of pipe. */
    close(pipefd[0]);

    while (!done) {
        fprintf(stderr, "Parent process (%ld): Tick.\n", (long)getpid());
        fflush(stderr);
        sleep(1);
        fprintf(stderr, "Parent process (%ld): Tock.\n", (long)getpid());
        fflush(stderr);
        sleep(1);

        /* Try reaping the child process. */
        p = waitpid(child, &status, WNOHANG);
        if (p == child || (p == (pid_t)-1 && errno == ECHILD)) {
            if (p == child && WIFSIGNALED(status))
                fprintf(stderr, "Child process died from %s. Parent will now exit, too.\n",
                        (WTERMSIG(status) == SIGINT) ? "SIGINT" :
                        (WTERMSIG(status) == SIGHUP) ? "SIGHUP" :
                        (WTERMSIG(status) == SIGTERM) ? "SIGTERM" : "an unknown signal");
            else
                fprintf(stderr, "Child process has exited, so the parent will too.\n");
            fflush(stderr);
            break;
        }
    }

    if (done) {
        fprintf(stderr, "Parent process (%ld): Exited due to %s.\n", (long)getpid(),
                   (done == SIGINT) ? "SIGINT" :
                   (done == SIGHUP) ? "SIGHUP" : "Unknown signal.\n");
        fflush(stderr);
    }

    /* Never reached! */
    return EXIT_SUCCESS;
}


#endif 
