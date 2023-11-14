#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd1[2]; // Pipe from P1 to P2
    int fd2[2]; // Pipe from P2 to P1

    char input_str[100]; // String input by the user
    pid_t p;

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    printf("Enter a string to send to P2:");
    scanf("%s", input_str);
    p = fork();

    if (p < 0) {
        fprintf(stderr, "fork Failed");
        return 1;
    }

    // P1 Process
    else if (p > 0) {
        close(fd1[0]); // Close reading end of the pipe from P1 to P2
        close(fd2[1]); // Close writing end of the pipe from P2 to P1

        // Write input string to P2 through fd1
        write(fd1[1], input_str, strlen(input_str) + 1);

        // Read concatenated string from P2 through fd2
        char concat_str[100];
        read(fd2[0], concat_str, 100);

        // Print the concatenated string received from P2
        printf("Concatenated string from P2: %s\n", concat_str);

        // Prompt user for a second input
        printf("Enter a second string to send to P1:");
        scanf("%s", input_str);

        // Write the second input string to P1 through fd2
        write(fd2[1], input_str, strlen(input_str) + 1);

        // Read final concatenated string from P2 through fd1
        char final_str[200];
        read(fd1[0], final_str, 200);

        // Concatenate "gobison.org" to the received string
        strcat(final_str, "gobison.org");

        // Print the final concatenated string
        printf("Final output from P1: %s\n", final_str);

        // Close both ends of pipes
        close(fd1[1]);
        close(fd2[0]);
    }

    // P2 Process
    else {
        close(fd1[1]); // Close writing end of the pipe from P1 to P2
        close(fd2[0]); // Close reading end of the pipe from P2 to P1

        // Read string from P1 through fd1
        char received_str[100];
        read(fd1[0], received_str, 100);

        // Concatenate "howard.edu" to the received string
        strcat(received_str, "howard.edu");

        // Write the concatenated string to P1 through fd2
        write(fd2[1], received_str, strlen(received_str) + 1);

        // Prompt user for a second input
        char second_input[100];
        printf("Enter a second string to send to P1:");
        scanf("%s", second_input);

        // Write the second input string to P1 through fd1
        write(fd1[1], second_input, strlen(second_input) + 1);

        // Close both ends of pipes
        close(fd1[0]);
        close(fd2[1]);

        exit(0);
    }

    return 0;
}
