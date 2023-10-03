/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int flag = 1; // Global variable to control the loop

void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
  flag = 0;
  exit(1); //exit after printing
}

int main(int argc, char * argv[])
{
  signal(SIGALRM,handler); //register handler to handle SIGALRM
  alarm(1); //Schedule a SIGALRM for 1 second

  while (flag) 
    { 
        // Busy wait for signal to be delivered
    }
  printf("Turing was right!\n");
  return 0; //never reached
}