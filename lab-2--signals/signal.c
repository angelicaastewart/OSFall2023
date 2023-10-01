/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int alarms = 0; // Global variable to count alarms
int flag = 0;   // Global variable to control looping in main

void handler(int signum)
{ 
  // Signal handler
    if (signum == SIGALRM)
    {
        alarms++; // Increment alarm count
        printf("Hello World!\n");
    }
    else if (signum == SIGINT)
    {
        printf("\nTotal execution time: %d seconds\n", alarms);
        exit(0);
    }
}

int main(int argc, char * argv[])
{
    signal(SIGALRM, handler); // Register handler to handle SIGALRM
    signal(SIGINT, handler);  // Register handler to handle SIGINT
    alarm(1);                 // Schedule a SIGALRM for 1 second

    while (1)
    {
        pause(); // Pause the program until a signal is received
    }

    return 0; // Never reached
}