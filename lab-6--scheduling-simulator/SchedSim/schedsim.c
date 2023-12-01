// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"


// Function to find the waiting time for all processes
void findWaitingTimeRR(ProcessType plist[], int n, int quantum)
{
    int bt_rem[n]; // Remaining burst time for each process

    for (int i = 0; i < n; i++)
        bt_rem[i] = plist[i].bt;

    int t = 0; // Current time

    while (1)
    {
        int done = 1;

        for (int i = 0; i < n; i++)
        {
            if (bt_rem[i] > 0)
            {
                done = 0; // There is a pending process

                if (bt_rem[i] > quantum)
                {
                    t += quantum;
                    bt_rem[i] -= quantum;
                }
                else
                {
                    t += bt_rem[i];
                    plist[i].wt = t - plist[i].bt;
                    bt_rem[i] = 0;
                }
            }
        }

        if (done == 1)
            break;
    }
}

// Function to find the waiting time for all processes
void findWaitingTimeSJF(ProcessType plist[], int n)
{
    int bt_rem[n]; // Remaining burst time for each process

    for (int i = 0; i < n; i++)
        bt_rem[i] = plist[i].bt;

    int t = 0; // Current time

    while (1)
    {
        int minP = -1; // Index of the process with the minimum remaining time

        for (int i = 0; i < n; i++)
        {
            if (bt_rem[i] > 0)
            {
                if (minP == -1 || bt_rem[i] < bt_rem[minP])
                    minP = i;
            }
        }

        if (minP == -1)
            break; // All processes are done

        t += 1;
        bt_rem[minP] -= 1;

        if (bt_rem[minP] == 0)
        {
            plist[minP].wt = t - plist[minP].bt;
        }
    }
}

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 

int my_comparer(const void *this, const void *that)
{

    ProcessType *p1 = (ProcessType *)this;
    ProcessType *p2 = (ProcessType *)that;

    if (p1->pri < p2->pri)
        return -1;
    else if (p1->pri > p2->pri)
        return 1;
    else
        return 0;
}

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

// Function to find the waiting time for all processes
void findavgTimePriority(ProcessType plist[], int n)
{
    // Sort processes based on priority
    qsort(plist, n, sizeof(ProcessType), my_comparer);

    // Function to find waiting time of all processes
    findWaitingTime(plist, n);

    // Function to find turn around time for all processes
    findTurnAroundTime(plist, n);

    // Display processes along with all details
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
    FILE *input_file = fopen(filename, "r");
    if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        fflush(stdout);
        exit(0);
    }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
       fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
       fflush(stdout);
       return 1;
     }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);

  // Priority
    n = 0;
    proc_list = initProc(argv[1], &n);

    findavgTimePriority(proc_list, n);

    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 