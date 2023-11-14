#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

#include "process.h"
#include "util.h"

#define DEBUG 0			//change this to 1 to enable verbose output

/**
 * Signature for a function pointer that can compare
 * You need to cast the input into its actual 
 * type and then compare them according to your
 * custom logic
 */
typedef int (*Comparer) (const void *a, const void *b);

/**
 * compares 2 processes
 * You can assume: 
 * - Process ids will be unique
 * - No 2 processes will have the same arrival time
 */
int my_comparer(const void *a, const void *b)
{
    const Process *processA = (const Process *)a;
    const Process *processB = (const Process *)b;

    // Compare based on priority first
    if (processA->priority < processB->priority)
        return -1;
    else if (processA->priority > processB->priority)
        return 1;

    // If priorities are equal, compare based on arrival time
    if (processA->arrival_time < processB->arrival_time)
        return -1;
    else if (processA->arrival_time > processB->arrival_time)
        return 1;

    // If both priority and arrival time are equal, consider the processes equal
    return 0;
}

int main(int argc, char *argv[])
{

    if (argc < 2) {
        fprintf(stderr, "Usage: ./func-ptr <input-file-path>\n");
        fflush(stdout);
        return 1;
    }

    /*******************/
    /* Parse the input */
    /*******************/
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        fflush(stdout);
        return 1;
    }

    Process *processes = parse_file(input_file);

    /*******************/
    /* sort the input  */
    /*******************/
    Comparer process_comparer = &my_comparer;

#if DEBUG
    for (int i = 0; i < P_SIZE; i++) {
        printf("%d (%d, %d) ",
                processes[i].pid,
                processes[i].priority, processes[i].arrival_time);
    }
    printf("\n");
#endif

    qsort(processes, P_SIZE, sizeof(Process), process_comparer);

    /**************************/
    /* print the sorted data  */
    /**************************/

    for (int i = 0; i < P_SIZE; i++) {
        printf("%d (%d, %d)\n",
                processes[i].pid,
                processes[i].priority, processes[i].arrival_time);
    }
    fflush(stdout);
    fflush(stderr);

    /************/
    /* clean up */
    /************/
    free(processes);
    fclose(input_file);
    return 0;
}
