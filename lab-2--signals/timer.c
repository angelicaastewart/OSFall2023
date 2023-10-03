/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//int flag = 1; // Global variable to control the loop
int alarm_count = 0; // Global variable to track the number of alarms

void sigint_handler(int signum)
{
    printf("\nTotal number of alarms: %d\n", alarm_count);
    printf("Total time executed: %d seconds\n", alarm_count);
    exit(EXIT_SUCCESS);
}

void sigalrm_handler(int signum)
{
    alarm_count++; // Increment alarm count on every SIGALRM signal
    printf("Alarm %d\n", alarm_count);
}

// void handler(int signum)
// { //signal handler
//   printf("Hello World!\n");
//   printf("Turing was right!\n");
//   //flag = 0;
//   alarm(1); // Schedule a SIGALRM for 1 second
//   //exit(1); //exit after printing
// }

int main(int argc, char * argv[])
{
  signal(SIGINT, sigint_handler); // Register SIGINT handler (CTRL-C)
  signal(SIGALRM, sigalrm_handler); // Register SIGALRM handler

  //signal(SIGALRM,handler); //register handler to handle SIGALRM
  alarm(1); //Schedule a SIGALRM for 1 second

  while (1) 
    { 
      pause(); // Wait for a signal to be delivered
    }

  return 0; //never reached
}