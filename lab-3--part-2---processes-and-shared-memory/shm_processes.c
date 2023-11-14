#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait>

// Function prototypes
void ParentProcess(int *);
void ChildProcess(int *);

int main(int argc, char *argv[]) {
    int ShmID;
    int *ShmPTR;
    pid_t pid;
    int status;

    if (argc != 5) {
        printf("Usage: %s #1 #2 #3 #4\n", argv[0]);
        exit(1);
    }

    // Create shared memory
    ShmID = shmget(IPC_PRIVATE, 4 * sizeof(int), IPC_CREAT | 0666);
    if (ShmID < 0) {
        printf("*** shmget error (server) ***\n");
        exit(1);
    }
    printf("Server has received a shared memory of four integers...\n");

    // Attach to shared memory
    ShmPTR = (int *)shmat(ShmID, NULL, 0);
    if (*ShmPTR == -1) {
        printf("*** shmat error (server) ***\n");
        exit(1);
    }
    printf("Server has attached the shared memory...\n");

    // Initialize shared memory
    ShmPTR[0] = atoi(argv[1]);
    ShmPTR[1] = atoi(argv[2]);
    ShmPTR[2] = atoi(argv[3]);
    ShmPTR[3] = atoi(argv[4]);
    printf("Server has filled %d %d %d %d in shared memory...\n",
           ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3]);

    printf("Server is about to fork a child process...\n");

    // Fork a child process
    pid = fork();
    if (pid < 0) {
        printf("*** fork error (server) ***\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        ChildProcess(ShmPTR);
        exit(0);
    }

    // Parent process
    ParentProcess(ShmPTR);

    // Wait for the child to finish
    wait(&status);

    // Detach from shared memory
    shmdt((void *)ShmPTR);
    printf("Server has detached its shared memory...\n");

    // Remove shared memory
    shmctl(ShmID, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");

    printf("Server exits...\n");
    exit(0);
}

// Parent process function
void ParentProcess(int *SharedMem) {
    srand(time(NULL));
    int account;

    for (int i = 0; i < 25; i++) {
        sleep(rand() % 6);  // Sleep between 0-5 seconds

        account = SharedMem[0];

        while (SharedMem[1] != 0)
            ; // Wait for child's turn

        if (account <= 100) {
            int balance = rand() % 101;

            if (balance % 2 == 0) {
                // Deposit money
                account += balance;
                printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
            } else {
                printf("Dear old Dad: Doesn't have any money to give\n");
            }

            SharedMem[0] = account;
        } else {
            printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
        }

        SharedMem[3] = 1;  // Set Turn to 1 (child's turn)
    }
}

// Child process function
void ChildProcess(int *SharedMem) {
    srand(time(NULL));
    int account;

    for (int i = 0; i < 25; i++) {
        sleep(rand() % 6);  // Sleep between 0-5 seconds

        account = SharedMem[0];

        while (SharedMem[3] != 1)
            ; // Wait for parent's turn

        int balance = rand() % 51;
        printf("Poor Student needs $%d\n", balance);

        if (balance <= account) {
            // Withdraw money
            account -= balance;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, account);
        } else {
            printf("Poor Student: Not Enough Cash ($%d)\n", account);
        }

        SharedMem[0] = account;

        SharedMem[1] = 0;  // Set Turn to 0 (parent's turn)
    }
}
