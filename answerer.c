					
 /* 
 	Defines a program that chooses a random integer,
	launches another program, and lets that program guess the integer.
*/

#include "headers.h"


pid_t guesserId;   //  Holds the process id of the process that guesses the number held by this ansewering process.

int	answer;    //  Holds the answer to be guessed.

int	binaryPlace;   //  Holds the binary place currently being guessed.

int	shouldContinue	= 1;    //  Holds '1' while this program should continue to run, or '0' otherwise.
  


//  Handles when the guesser correctly guess that digit 'digit' is at binary position 'binaryPlace'.
void correctGuess (int digit){

    if  (digit)
      printf("Answerer: \"Yes, there is a %d.\"\n",binaryPlace);
    else
      printf("Answerer: \"Yes, there is no %d.\"\n",binaryPlace);

    binaryPlace = binaryPlace >> 1;

    //  Tell the guessing process that it guessed correctly

    kill(guesserId, CORRECT_SIG);
}


//  Handle when the guesser incorrectly guess that digit 'digit' is at binary position 'binaryPlace'.
void incorrectGuess	(int digit){

    printf("Answerer: \"Sorry, guess again.\"\n");
    
    //  Tell the guessing process that it guessed incorrectly

    kill(guesserId, INCORRECT_SIG);
}


//  Handles when the guesser guesses that a '0' is at binary position 'binaryPlace'.  Ignores 'sig'.
void zeroSigHandler	(int sig){

    if  ( (answer & binaryPlace) == 0 )
      correctGuess(0);
    else
      incorrectGuess(0);
}


//  Handles when the guesser guesses that a '1' is at binary position 'binaryPlace'.  Ignores 'sig'. 
void oneSigHandler (int sig){

    if  ( (answer & binaryPlace) == binaryPlace )
      correctGuess(1);
    else
      incorrectGuess(1);
}


//  handle when the timing process tells this process that the time is up.  Ignores 'sig'.  
void sigIntHandler (int	sig){

    printf("Answerer: \"Whoops, time is up!\"\n");
    shouldContinue	= 0;

    //  Tell the guessing process that the time is up by sending it SIGINT

    kill(guesserId, SIGINT);
}



int	main (){

    srand(getpid());
    rand();
    answer	= rand() % 128;
    binaryPlace	= 1 << 7;

    printf("Answerer: \"(Don't tell, but the answer is %d)\"\n", answer);


    struct sigaction one_sig;  //  Install signal handlers
    struct sigaction zero_sig;

    memset(&one_sig, '\0', sizeof(one_sig));
    one_sig.sa_handler  = oneSigHandler;
    sigaction(ONE_SIG, &one_sig, NULL);

    memset(&zero_sig, '\0', sizeof(zero_sig));
    zero_sig.sa_handler  = zeroSigHandler;
    sigaction(ZERO_SIG, &zero_sig, NULL);


    if ((guesserId = fork()) == 0){						//  Start guessing process, put process id in 'guesserId'
            execl(GUESSER_PROG, GUESSER_PROG, NULL);
            printf("Sorry cannot run %s\n", "guesser");
            exit(EXIT_FAILURE);
    }


    while  (shouldContinue && (binaryPlace > 0))		// Waiting for guessing process
      sleep(1);

    
    return(EXIT_SUCCESS);
}












