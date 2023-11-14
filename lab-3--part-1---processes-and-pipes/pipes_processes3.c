#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <grep_argument>\n", argv[0]);
    return 1;
  }

  int pipefd1[2];
  int pipefd2[2];
  int pid1, pid2;

  // make pipes
  pipe(pipefd1);
  pipe(pipefd2);

  pid1 = fork();

  if (pid1 == 0)
  {
    // Child process (P2)
    dup2(pipefd1[0], 0);  // Replace standard input with the input part of pipe1
    dup2(pipefd2[1], 1);  // Replace standard output with the output part of pipe2

    // Close unused ends of pipes
    close(pipefd1[1]);
    close(pipefd2[0]);

    char *grep_args[] = {"grep", argv[1], NULL};
    execvp("grep", grep_args);
  }
  else
  {
    pid2 = fork();

    if (pid2 == 0)
    {
      // Child process (P3)
      dup2(pipefd2[0], 0);  // Replace standard input with the input part of pipe2

      // Close unused ends of pipes
      close(pipefd2[1]);

      execlp("sort", "sort", (char *)NULL);
    }
    else
    {
      // Parent process (P1)
      dup2(pipefd1[1], 1);  // Replace standard output with the output part of pipe1

      // Close unused ends of pipes
      close(pipefd1[0]);
      close(pipefd2[0]);
      close(pipefd2[1]);

      char *cat_args[] = {"cat", "scores", NULL};
      execvp("cat", cat_args);
    }
  }

  return 0;
}
