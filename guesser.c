/*
	Attempts to guess a random integer
*/

#include	"headers.h"


int	answer;       
int	binaryPlace;     //  hold the current binary position being queried.
int	shouldContinue	= 1;    //  '1' while this program should continue to run, or '0' otherwise.
int	count = 0;     //  the number of guesses that have been executed for the current value of 'binaryPlace'.


//  informs the answering process of our first guess for the digit at binary place 'binaryPlace'.  
void firstGuess(){

    printf("Guesser: \"Is there a %d?\"\n", binaryPlace);
    sleep(1);
    
    //  Tell the answering (parent) process our guess is '1'

    kill(getppid(), ONE_SIG);
}


//  Informs the answering process of our second guess (if necessary) for the digit at binary place 'binaryPlace'.  
void secondGuess	(){

    count++;
    printf("Guesser: \"Whoops, I guess there is no %d.\"\n",binaryPlace);
    sleep(1);

    //  Tell the answering (parent) process our guess is '0'

    kill(getppid(), ZERO_SIG);
}


//  Handles when the answering process informs us that the latest guess was correct.  Ignores 'sig'.  
void correctSigHandler (int	sig){

    if  (count == 0)
    {
      answer   	= answer | binaryPlace;
      printf("Guesser: \"So far I have %d\"\n",answer);
    }

    count		= 0;
    binaryPlace	= binaryPlace >> 1;

    if  (binaryPlace == 0)
      printf("Guesser: \"Yay!  I got the answer!  It is %d\"\n",answer);
    else
      firstGuess();
}


//  Handles when the answering process informs us that the latest guess was incorrect.  Ignores 'sig'. 
void	incorrectSigHandler(int	sig){
      secondGuess();
}


//  Handles when the answering process tells us that time time is up.  Ignores 'sig'.  
void	sigIntHandler	(int	sig){

    printf("Guesser: \"Aw man!  And I was close, too!\"\n");
    shouldContinue	= 0;
}



int	main (){

      answer	= 0;
      binaryPlace	= 1 << 7;

      struct sigaction correct;
      struct sigaction incorrect;

      memset(&correct, '\0', sizeof(correct));
      correct.sa_handler  = correctSigHandler;
      sigaction(CORRECT_SIG, &correct, NULL);

      memset(&incorrect, '\0', sizeof(incorrect));
      incorrect.sa_handler  = incorrectSigHandler;
      sigaction(INCORRECT_SIG, &incorrect, NULL);


      firstGuess();

      while  (shouldContinue && (binaryPlace > 0))
        sleep(1);

      return(EXIT_SUCCESS);
}




