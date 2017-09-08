

#include "headers.h"

int	isTimeUp = 0;   //  Holds '1' when the time is up, or '0' before then.
int	didChildStop = 0;   //  Holds '1' after the child has stopped, or '0' before then.


void sigChildHandler (int sig){         // handle when the child process has stopped by unzombie-ing it. Ignores 'sig'.
  
  pid_t pid;

  while((pid = wait(NULL)) > 0){        //  wait for the child so it does not stay a zombie process

      if  (isTimeUp)
          printf("Timer: \"Sorry, time is up!\"\n");
      else
          printf("Timer: \"Congratulations!\"\n");

      didChildStop  = 1;
  }
  
}


int	main (){        
 
    pid_t	childId;

    struct sigaction  act;

    memset(&act, '\0', sizeof(act));
    act.sa_handler  = sigChildHandler;
    sigaction(SIGCHLD, &act, NULL);
    
    if ((childId = fork()) == 0){
            execl(ANSWERER_PROG, ANSWERER_PROG, NULL);
            printf("Sorry cannot run %s\n", "answerer");
            exit(EXIT_FAILURE);
    }
    
    int secs;

    for  (secs = TIME_LIMIT_NUM_SECONDS;  (secs > 0) && !didChildStop;  secs--){
      printf("Timer: \"%d seconds\"\n",secs);
      sleep(1);
    }

    isTimeUp	= 1;

    if  (!didChildStop)
    {
      printf("Timer: \"%d seconds\"\n",secs);
      
      kill(childId, SIGINT);  //  Stop the child process by sending it SIGINT
    }

    while  ( !didChildStop )
      sleep(1);

    return(EXIT_SUCCESS);
}




