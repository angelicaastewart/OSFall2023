/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//int flag = 1; // Global variable to control the loop

void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
  printf("Turing was right!\n");
  //flag = 0;
  alarm(1); // Schedule a SIGALRM for 1 second
  //exit(1); //exit after printing
}

int main(int argc, char * argv[])
{
  signal(SIGALRM,handler); //register handler to handle SIGALRM
  alarm(1); //Schedule a SIGALRM for 1 second

  while (1) 
    { 
      pause(); // Wait for a signal to be delivered
    }

  return 0; //never reached
}