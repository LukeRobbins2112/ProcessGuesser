

#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<signal.h>
#include	<string.h>
#include	<sys/wait.h>



#define		GUESSER_PROG		"guesser"   
#define		ANSWERER_PROG		"answerer"  



const int TIME_LIMIT_NUM_SECONDS = 16;  

const int INCORRECT_SIG = SIGUSR1; 

const int CORRECT_SIG = SIGUSR2;  

const int ZERO_SIG = SIGUSR1;  

const int ONE_SIG	= SIGUSR2;  








