#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;

// Signal handler for SIGINT (Ctrl-C)
void sigint_handler(int signum) {
    // Print a new line after the interrupted command
    printf("\n%s", prompt);
    fflush(stdout);
}

bool process_timed_out = false;

// Signal handler for SIGALRM
void sigalarm_handler(int signum) {
    process_timed_out = true;
    printf("\nProcess timed out.\n");
    fflush(stdout);
    exit(EXIT_FAILURE);
}

// Function to handle built-in commands
bool handle_builtin_commands(char **arguments) {
    if (arguments[0] == NULL) {
        // No command provided
        return true;
    }

    // Handle built-in commands: cd, exit, pwd, echo, env, setenv
    if (strcmp(arguments[0], "cd") == 0) {
        // Change directory
        if (arguments[1] != NULL) {
            if (chdir(arguments[1]) != 0) {
                perror("cd");
            }
        } else {
            // Handle 'cd' without arguments (go to home directory)
            chdir(getenv("HOME"));
        }
    } else if (strcmp(arguments[0], "exit") == 0) {
        // Exit the shell
        exit(0);
    } else if (strcmp(arguments[0], "pwd") == 0) {
        // Print current working directory
        char cwd[MAX_COMMAND_LINE_LEN];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("getcwd");
        }
    } else if (strcmp(arguments[0], "echo") == 0) {
        // Print message and values of environment variables
        int i = 1;
        while (arguments[i] != NULL) {
            printf("%s ", arguments[i]);
            i++;
        }
        printf("\n");
    } else if (strcmp(arguments[0], "env") == 0) {
        // Print current environment variables
        char **env = environ;
        while (*env) {
            printf("%s\n", *env);
            env++;
        }
    } else if (strcmp(arguments[0], "setenv") == 0) {
        // Set environment variable
        if (arguments[1] != NULL && arguments[2] != NULL) {
            if (setenv(arguments[1], arguments[2], 1) != 0) {
                perror("setenv");
            }
        } else {
            printf("Usage: setenv <variable> <value>\n");
        }
    } else {
        // Not a built-in command, not handled
        return false;
    }

    return true;
}

int main() {
    char command_line[MAX_COMMAND_LINE_LEN]; // Stores the input command line
    char *arguments[MAX_COMMAND_LINE_ARGS]; // Stores tokenized command line arguments
    int background_process_count = 1; // Counter for background processes

    // Register signal handlers for SIGALRM and SIGINT
    signal(SIGALRM, sigalarm_handler);
    signal(SIGINT, sigint_handler);

    while (true) {
        // Print the shell prompt with the current working directory
        char cwd[MAX_COMMAND_LINE_LEN];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s%s", cwd, prompt);
            fflush(stdout);
        } else {
            perror("getcwd error");
            exit(0);
        }

        // Read input from stdin and store it in command_line
        if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
            fprintf(stderr, "fgets error");
            exit(0);
        }

        // Skip empty lines (when just ENTER is pressed)
        while (command_line[0] == 0x0A) {
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
        }

        // Tokenize the command line input
        char *token = strtok(command_line, delimiters);
        int arg_index = 0; // Index to store tokens in the arguments array

        // Tokenize the input and store tokens in the arguments array
        while (token != NULL && arg_index < MAX_COMMAND_LINE_ARGS - 1) {
            arguments[arg_index] = token;
            arg_index++;

            // Get the next token
            token = strtok(NULL, delimiters);
        }

        // If the user input was EOF (ctrl+d), exit the shell
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // Handle built-in commands, if any
        if (!handle_builtin_commands(arguments)) {
            // Create a child process
            pid_t child_pid = fork();

            if (child_pid == -1) {
                // Error occurred while forking
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (child_pid == 0) {
                // Child process

                // Set an alarm for 10 seconds
                alarm(10);

                // Execute the command in the child process
                if (execvp(arguments[0], arguments) == -1) {
                    // Error occurred while executing the command
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            } else {
                // Parent process

                // Wait for the child process to complete, unless it's a background process
                int status;
                if (arguments[arg_index - 1] != NULL && strcmp(arguments[arg_index - 1], "&") == 0) {
                    // Background process: don't wait
                    printf("[%d] %d\n", background_process_count, child_pid);
                    background_process_count++;
                } else {
                    // Foreground process: wait for the child to complete
                    waitpid(child_pid, &status, 0);

                    // Cancel the alarm if the process completed before the timer expiration
                    alarm(0);

                    // Check if the process was terminated due to timeout
                    if (process_timed_out) {
                        process_timed_out = false;
                        continue; // Skip the rest of the loop iteration if the process was terminated
                    }
                }
            }
        }

        // Check for output redirection symbol ('>')
        int output_redirect_index = -1;
        for (int i = 0; i < arg_index; ++i) {
            if (strcmp(arguments[i], ">") == 0) {
                output_redirect_index = i;
                break;
            }
        }

        if (output_redirect_index != -1) {
            // Open the file for writing (create or truncate)
            int fd_output = open(arguments[output_redirect_index + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (fd_output == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            // Redirect stdout to the output file
            if (dup2(fd_output, STDOUT_FILENO) == -1) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            // Remove the redirection symbols and file name from arguments
            arguments[output_redirect_index] = NULL;
            arguments[output_redirect_index + 1] = NULL;
        }

        // Check for input redirection symbol ('<')
        int input_redirect_index = -1;
        for (int i = 0; i < arg_index; ++i) {
            if (strcmp(arguments[i], "<") == 0) {
                input_redirect_index = i;
                break;
            }
        }

        if (input_redirect_index != -1) {
            // Open the file for reading
            int fd_input = open(arguments[input_redirect_index + 1], O_RDONLY);
            if (fd_input == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            // Redirect stdin from the input file
            if (dup2(fd_input, STDIN_FILENO) == -1) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }

            // Remove the redirection symbols and file name from arguments
            arguments[input_redirect_index] = NULL;
            arguments[input_redirect_index + 1] = NULL;
        }

        // Check for pipe symbol ('|')
        int pipe_index = -1;
        for (int i = 0; i < arg_index; ++i) {
            if (strcmp(arguments[i], "|") == 0) {
                pipe_index = i;
                break;
            }
        }

        if (pipe_index != -1) {
            // Split arguments into two parts: before and after the pipe
            arguments[pipe_index] = NULL;
            char **first_command = arguments;
            char **second_command = arguments + pipe_index + 1;

            // Create a pipe
            int pipe_fd[2];
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            pid_t pid1, pid2;

            // Fork the first child process
            if ((pid1 = fork()) == 0) {
                // Child process 1
                close(pipe_fd[0]); // Close unused read end of the pipe
                dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
                close(pipe_fd[1]); // Close the write end of the pipe

                // Execute the first command
                execvp(first_command[0], first_command);
                perror("execvp (first command)");
                exit(EXIT_FAILURE);
            } else if (pid1 > 0) {
                // Parent process

                // Fork the second child process
                if ((pid2 = fork()) == 0) {
                    // Child process 2
                    close(pipe_fd[1]); // Close unused write end of the pipe
                    dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
                    close(pipe_fd[0]); // Close the read end of the pipe

                    // Execute the second command
                    execvp(second_command[0], second_command);
                    perror("execvp (second command)");
                    exit(EXIT_FAILURE);
                } else if (pid2 > 0) {
                    // Parent process

                    // Close both ends of the pipe in the parent
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);

                    // Wait for both child processes to complete
                    waitpid(pid1, NULL, 0);
                    waitpid(pid2, NULL, 0);
                } else {
                    perror("fork (second child)");
                    exit(EXIT_FAILURE);
                }
            } else {
                perror("fork (first child)");
                exit(EXIT_FAILURE);
            }
        }
    }

    // This should never be reached.
    return -1;
}
