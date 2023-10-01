#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    srand(time(NULL)); // Seed for random number generation

    // Fork first child process
    pid_t child1 = fork();

    if (child1 == 0) {
        // Inside first child process
        int iterations = rand() % 30 + 1; // Random number of iterations (1 to 30)
        for (int i = 0; i < iterations; ++i) {
            printf("Child Pid: %d is going to sleep!\n", getpid());
            sleep(rand() % 10 + 1); // Sleep for random time (1 to 10 seconds)
            printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
        }
        exit(0); // Terminate the child process
    }

    // Fork second child process
    pid_t child2 = fork();

    if (child2 == 0) {
        // Inside second child process
        int iterations = rand() % 30 + 1; // Random number of iterations (1 to 30)
        for (int i = 0; i < iterations; ++i) {
            printf("Child Pid: %d is going to sleep!\n", getpid());
            sleep(rand() % 10 + 1); // Sleep for random time (1 to 10 seconds)
            printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
        }
        exit(0); // Terminate the child process
    }

    // Parent process
    wait(NULL); // Wait for the first child to complete
    printf("Child Pid: %d has completed\n", child1);
    wait(NULL); // Wait for the second child to complete
    printf("Child Pid: %d has completed\n", child2);

    return 0;
}
